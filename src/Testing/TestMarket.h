#if !defined _TESTMARKET_H_
#define _TESTMARKET_H_

#include "MarketModel.h"
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/// This is the simplest possible Market, for testing.
class TestMarket : public MarketModel {
  public :
    TestMarket(std::string commod) {
      commodity_ = commod;
    }
    virtual void receiveMessage(Message* msg) {
    }
    virtual void resolve() {
    }
    virtual void copy(TestMarket* src){
      commodity_ = src->commodity_;
    }
    void copyFreshModel(Model* src){
      copy(dynamic_cast<TestMarket*>(src));
    }
};
#endif