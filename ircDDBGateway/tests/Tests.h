#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BROWN   "\033[33m"

#include <wx/wxprec.h>
#include <wx/wx.h>

#include "../ircDDB/IRCDDBApp.h"
#include "../Common/APRSWriterThread.h"
#include "../Common/SlowDataEncoder.h"

class APRSWriterThreadTests {
  CAPRSWriterThread *m_thread;
 public:
 APRSWriterThreadTests(CAPRSWriterThread *thread): m_thread(thread){}
  bool testGetAPRSPassword();
  bool testDeconstructor();
};

class IRCDDBAppTest {
  IRCDDBApp *m_app;
 public:
 IRCDDBAppTest(IRCDDBApp *app): m_app(app){}
  bool testDoUpdate();
};

class SlowDataEncoderTests {
  CSlowDataEncoder *m_encoder;
 public:
 SlowDataEncoderTests(CSlowDataEncoder *encoder): m_encoder(encoder){}
  bool testSetGPSData();
};
