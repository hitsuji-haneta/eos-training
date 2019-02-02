#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio:contract]] addressbook : public eosio::contract {
  public:
    addressbook(
      name receiver,
      name code,
      datastream<const char*> ds
    ):contract(receiver, code, ds) {}

    void upsert(
      name user,
      std::string first_name,
      std::string last_name,
      std::string street,
      std::string city,
      std::string state
    ) {
      require_auth( user );
      address_index addresses(_code, _code.value);
      auto iterator = addresses.find(user.value);
      if( iterator == addreseses.end()) {
        addresses.emplace( user, [&]( auto& row ) {
          row.key = user;
          row.first_name = first_name;
          row.last_name = last_name;
          row.street = street;
          row.city = city;
          row.state = state;
        });
      } else {
        std::string changes;
        addresses.modify(iterator, user, [&]( auto& row ) {
          row.key = user;
          row.first_name = first_name;
          row.last_name = last_name;
          row.street = street;
          row.city = city;
          row.state = state;
        });
      }
    }

  private:
    struct person {
      name key;
      std::string first_name;
      std::string last_name;
      std::string street;
      std::string city;
      std::string state;

      uint64_t primary_key() const { return key.value; }
    };

    typedef eosio::multi_index<"people"_n, person> address_index;
}