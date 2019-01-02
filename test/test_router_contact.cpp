#include <gtest/gtest.h>

#include <router_contact.hpp>
#include <crypto.hpp>

static const byte_t DEF_VALUE[] = "unittest";

struct RCTest : public ::testing::Test
{
  using RC_t     = llarp::RouterContact;
  using SecKey_t = llarp::SecretKey;

  static void
  SetUpTestCase()
  {
    llarp::NetID::DefaultValue = DEF_VALUE;
  }

  RCTest() : crypto(llarp::Crypto::sodium{}), oldval(llarp::NetID::DefaultValue)
  {
    rc.Clear();
  }

  ~RCTest()
  {
    llarp::NetID::DefaultValue = oldval;
  }

  RC_t rc;
  llarp::Crypto crypto;
  const byte_t* oldval;
};

TEST_F(RCTest, TestSignVerify)
{
  SecKey_t encr;
  SecKey_t sign;
  crypto.encryption_keygen(encr);
  crypto.identity_keygen(sign);
  rc.enckey = encr.toPublic();
  ASSERT_TRUE(rc.Sign(&crypto, sign));
  ASSERT_TRUE(rc.Verify(&crypto, llarp::time_now_ms()));
}
