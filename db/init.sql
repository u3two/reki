CREATE TABLE packets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    arrival_date INTEGER NOT NULL,
    data BLOB
);

CREATE TABLE ethernet_packets (
    packet_id INTEGER NOT NULL,
    destination_address TEXT NOT NULL
    source_address TEXT NOT NULL
    FOREIGN KEY(packet_id) REFERENCES packets(id) ON DELETE CASCADE
);

CREATE TABLE ip_packets (
    packet_id INTEGER NOT NULL,
    destination_address TEXT NOT NULL
    source_address TEXT NOT NULL
    FOREIGN KEY(packet_id) REFERENCES packets(id) ON DELETE CASCADE
);
