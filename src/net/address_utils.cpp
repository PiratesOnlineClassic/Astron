#include "address_utils.h"

#include <algorithm>
#include <boost/asio.hpp>

using namespace boost::asio::ip;

static bool split_port(std::string &ip, uint16_t &port)
{
    size_t last_colon = ip.rfind(':');
    if(last_colon == std::string::npos) {
        return true;
    }

    // If the input contains *multiple* colons, it's an IPv6 address.
    // We ignore these unless the IPv6 address is bracketed and the port
    // specification occurs outside of the brackets.
    // (e.g. "[::]:1234")
    if(std::count(ip.begin(), ip.end(), ':') > 1) {
        // Yep, we're assuming IPv6. Let's see if the last colon has a
        // ']' before it.
        // Note that this still can lead to weird inputs getting
        // through, but that'll get caught by a later parsing step. :)
        if(ip[last_colon - 1] != ']') {
            return true;
        }
    }

    try {
        port = std::stoi(ip.substr(last_colon + 1));
        ip = ip.substr(0, last_colon);
    } catch(std::invalid_argument) {
        return false;
    }

    return true;
}

static address parse_address(const std::string &ip_str, boost::system::error_code &ec)
{
    if(ip_str[0] == '[' && ip_str[ip_str.length() - 1] == ']') {
        return boost::asio::ip::make_address(ip_str.substr(1, ip_str.length() - 2), ec);
    } else {
        return boost::asio::ip::make_address(ip_str, ec);
    }
}

static bool validate_hostname(const std::string &hostname)
{
    // Standard hostname rules (a la RFC1123):
    // - Must consist of only the characters: A-Z a-z 0-9 . -
    // - Each dot-separated section must contain at least one character.
    // - The - character may not appear twice.
    // - Hostname sections may not begin or end with a -
    //
    // In other words, we must not have a "..", "-.", or ".-" anywhere.
    // We also have to verify that the beginning of the hostname is not a
    // "-" or "." and that the end is not a "-".
    // Everything else is accepted as a valid hostname.

    for(auto it = hostname.begin(); it != hostname.end(); ++it) {
        if(*it != '.' && *it != '-' && !isalnum(*it)) {
            return false;
        }
    }

    if(hostname.find("..") != std::string::npos ||
       hostname.find("-.") != std::string::npos ||
       hostname.find(".-") != std::string::npos) {
        return false;
    }

    if(hostname[0] == '-' || hostname[0] == '.' || hostname[0] == '\0') {
        return false;
    }

    if(hostname[hostname.length() - 1] == '-') {
        return false;
    }

    return true;
}

bool is_valid_address(const std::string &hostspec)
{
    std::string host = hostspec;
    uint16_t port;

    if(!split_port(host, port)) {
        return false;
    }

    boost::system::error_code ec;
    parse_address(host, ec);

    if(ec.value() == 0) {
        return true;
    } else {
        return validate_hostname(host);
    }
}

std::vector<tcp::endpoint> resolve_address(
    const std::string &hostspec, uint16_t port,
    boost::asio::io_context &io_service, boost::system::error_code &ec)
{
    std::vector<tcp::endpoint> ret;

    std::string host = hostspec;

    if(!split_port(host, port)) {
        return ret;
    }

    address addr = parse_address(host, ec);
    if(ec.value() == 0) {
        tcp::endpoint ep(addr, port);
        ret.push_back(ep);
    } else {
        tcp::resolver resolver(io_service);
        auto results = resolver.resolve(host, std::to_string(port), ec);

        for(auto& ep : results) {
            ret.push_back(ep);
        }
    }

    return ret;
}
