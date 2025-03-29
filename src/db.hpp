#ifndef REKI_DB_HPP
#define REKI_DB_HPP

/// Fetch packets into the packet store
void db_fetch(const char *db_string);

/// Insert packets from the packet store into the database
void db_store(const char *db_string);

#endif /* REKI_DB_HPP */
