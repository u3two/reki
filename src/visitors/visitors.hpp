#ifndef REKI_VISITOR
#define REKI_VISITOR

#include "../packet.hpp"
#include "../protocols/ethernet.hpp"
#include "../protocols/ip.hpp"
#include "../protocols/tcp.hpp"
#include "../protocols/udp.hpp"
#include "../protocols/arp.hpp"
#include "../protocols/icmp.hpp"

#include "../gui/listing.hpp"
#include "../gui/explorer.hpp"

#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

namespace visitors {

class PacketVisitor {
public:
    virtual ~PacketVisitor() {}

    virtual void visit(Packet& a) = 0;
    virtual void visit(EthernetPacket& a) = 0;
    virtual void visit(IP_Packet& a) = 0;
    virtual void visit(TCP_Packet& a) = 0;
    virtual void visit(UDP_Packet& a) = 0;
    virtual void visit(ARP_Packet& a) = 0;
    virtual void visit(ICMP_Packet& a) = 0;
};


#define VISITOR_FUNCTIONS \
    void visit(Packet& a) override; \
    void visit(EthernetPacket& a) override; \
    void visit(IP_Packet& a) override; \
    void visit(TCP_Packet& a) override; \
    void visit(UDP_Packet& a) override; \
    void visit(ARP_Packet& a) override; \
    void visit(ICMP_Packet& a) override; \


class Printer final : public PacketVisitor {
public:
    bool hexdump = false;

    VISITOR_FUNCTIONS
};

class DatabaseStore final : public PacketVisitor {
private:
    soci::session m_sql;
public:
    explicit DatabaseStore(const char *file)
    : m_sql(soci::sqlite3, file) {}

    VISITOR_FUNCTIONS
};

class ListingData final : public PacketVisitor {
private:
    gui::ListingData m_listing {};
public:
    const gui::ListingData &get_listing() { return this->m_listing; };

    VISITOR_FUNCTIONS
};

class ExplorerData final : public PacketVisitor {
public:
    std::vector<gui::ExplorerData> items;

    VISITOR_FUNCTIONS
};

}

#endif /* REKI_VISITOR */
