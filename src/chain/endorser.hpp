#ifndef TETHYS_PUBLIC_MERGER_ENDORSER_HPP
#define TETHYS_PUBLIC_MERGER_ENDORSER_HPP

#include "../config/types.hpp"

namespace tethys {
class Endorser {
public:
  base58_type endorser_id;
  std::string endorser_pk;
  base64_type endorser_sig;
  base64_type endorser_agga;
  base64_type endorser_aggz;

  Endorser() = default;
  Endorser(base58_type &&endorser_id_, string &&endorser_pk_, base64_type &&endorser_sig_)
    : endorser_id(endorser_id_), endorser_pk(endorser_pk_), endorser_sig(endorser_sig_) {}
  Endorser(base58_type &endorser_id_, string &endorser_pk_, base64_type &endorser_sig_)
    : endorser_id(endorser_id_), endorser_pk(endorser_pk_), endorser_sig(endorser_sig_) {}

  Endorser(base58_type &&endorser_id_, string &&endorser_pk_, base64_type &&endorser_agga_, base64_type &&endorser_aggz_)
    : endorser_id(endorser_id_), endorser_pk(endorser_pk_), endorser_agga(endorser_agga_), endorser_aggz(endorser_aggz_) {}
  Endorser(base58_type &endorser_id_, string &endorser_pk_, base64_type &endorser_agga_, base64_type &endorser_aggz_)
    : endorser_id(endorser_id_), endorser_pk(endorser_pk_), endorser_agga(endorser_agga_), endorser_aggz(endorser_aggz_) {}
};
} // namespace tethys

#endif