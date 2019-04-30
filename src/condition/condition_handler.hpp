#ifndef GRUUTSCE_CONDITION_HANDLER_HPP
#define GRUUTSCE_CONDITION_HANDLER_HPP

#include "../config.hpp"
#include "base_condition_handler.hpp"
#include "handler_compare.hpp"
#include "handler_signature.hpp"
#include "handler_certificate.hpp"
#include "handler_time.hpp"
#include "handler_endorser.hpp"
#include "handler_user.hpp"

#include <algorithm>
#include <iostream>
#include <map>

namespace gruut::gsce {

class ConditionHandler : BaseConditionHandler {

public:
  ConditionHandler() = default;

  bool evalue(pugi::xml_node &doc_node, Datamap &datamap) override {
    PrimaryConditionType base_condition_type = getPrimaryConditionType(doc_node.name());
    EvalRuleType base_eval_rule = getEvalRule(doc_node.attribute("eval-rule").value());

    bool eval_result = true;

    switch(base_condition_type) {
    case PrimaryConditionType::ROOT:
    case PrimaryConditionType::IF:
    case PrimaryConditionType::NIF: {
      if (base_eval_rule == EvalRuleType::AND) {
        eval_result = true;
        for (pugi::xml_node &tags: doc_node) {
          eval_result &= evalue(tags, datamap);
          if (!eval_result)
            break;
        }
      } else {
        eval_result = false;
        for (pugi::xml_node &tags: doc_node) {
          eval_result |= evalue(tags, datamap);
          if (eval_result)
            break;
        }
      }
      break;
    }
    case PrimaryConditionType::COMPARE: {
      CompareHandler compare_handler;
      eval_result = compare_handler.evalue(doc_node, datamap);
      break;
    }
    case PrimaryConditionType::SIGNATURE: {
      SignatureHandler signature_handler;
      eval_result = signature_handler.evalue(doc_node, datamap);
      break;
    }
    case PrimaryConditionType::CERTIFICATE: {
      CertificateHandler certificate_handler;
      eval_result = certificate_handler.evalue(doc_node, datamap);
      break;
    }
    case PrimaryConditionType::TIME: {
      TimeHandler time_handler;
      eval_result = time_handler.evalue(doc_node, datamap);
      break;
    }
    case PrimaryConditionType::ENDORSER: {
      EndorserHandler endorser_handler;
      eval_result = endorser_handler.evalue(doc_node, datamap);
      break;
    }
    case PrimaryConditionType::RECEIVER:
    case PrimaryConditionType::USER: {
      UserHandler user_handler;
      user_handler.setUserType(base_condition_type);
      eval_result = user_handler.evalue(doc_node, datamap);
      break;
    }
    default:
      break;
    }

    if(base_condition_type == PrimaryConditionType::NIF) {
      eval_result = !eval_result;
    }

    return eval_result;
  }

};

}

#endif //GRUUTSCE_CONDITION_HANDLER_HPP