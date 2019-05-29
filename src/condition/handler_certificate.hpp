#ifndef TETHYS_SCE_HANDLER_CERTIFICATE_HPP
#define TETHYS_SCE_HANDLER_CERTIFICATE_HPP

#include <botan-2/botan/oids.h>
#include <botan-2/botan/rsa.h>
#include "../config.hpp"
#include "base_condition_handler.hpp"

namespace tethys::tsce {

class CertificateHandler : public BaseConditionHandler {
public:
  CertificateHandler() = default;

  bool evalue(pugi::xml_node &doc_node, DataManager &data_manager) override {
    auto pk_node = doc_node.child("pk");
    auto by_node = doc_node.child("by");

    if(pk_node.empty() || by_node.empty())
      return false;

    std::string pk = pk_node.attribute("value").value();
    std::string pk_type = pk_node.attribute("type").value();

    if(pk.empty()){
      return false;
    } else {
      pk = data_manager.eval(pk);
      if(pk.empty())
        return false;
    }

    std::string by_attr_val = by_node.attribute("value").value();
    std::string by_type = by_node.attribute("type").value();
    std::string by_val = !by_attr_val.empty() ?  data_manager.eval(by_attr_val) : by_node.text().as_string();

    if(by_val.empty())
      return false;

    //TODO : may be handled by type ( `PEM` / `DER` )
    //now just check `PEM`
    try {
      Botan::DataSource_Memory by_cert_datasource(by_val);
      Botan::DataSource_Memory cert_datasource(pk);

      Botan::X509_Certificate by_cert(by_cert_datasource);
      Botan::X509_Certificate cert(cert_datasource);

      std::string algo_name = Botan::OIDS::oid2str(by_cert.subject_public_key_algo().get_oid());

      if(algo_name.size() < 3) {
        return false;
      }

      ssize_t pos;
      if ( (pos = algo_name.find("RSA")) != std::string::npos && pos == 0) {
        Botan::RSA_PublicKey by_pk(by_cert.subject_public_key_algo(), by_cert.subject_public_key_bitstring());
        return cert.check_signature(by_pk);
      } else if ((pos = algo_name.find("ECDSA")) != std::string::npos && pos == 0) {
        Botan::ECDSA_PublicKey by_pk(by_cert.subject_public_key_algo(), by_cert.subject_public_key_bitstring());
        return cert.check_signature(by_pk);
      } else {
        return false; // not supported!
      }
    }
    catch(Botan::Exception &exception){
      return false;
    }
  }
};

}

#endif
