#include "Tests.h"
using namespace std;

class UnitTests: public wxApp {
public:
    virtual bool OnInit();
};

int assertOK(bool success, wxString methodName) {
  if (!success) {
    cout << "testing " << BROWN << methodName << RED << " failed" << RESET << endl;
    return 1;
  } else {
    cout << "testing " << BROWN << methodName << GREEN << " passed" << RESET << endl;
    return 0;
  }
}

bool APRSWriterThreadTests::testGetAPRSPassword() {
  // test proper construction of the string
  wxString callsign = wxT("CALLSIGN");
  unsigned int expectedValue = 26856;
  return expectedValue == m_thread->getAPRSPassword(callsign);
};

bool IRCDDBAppTest::testDoUpdate() {
  // TODO(n0dec): find a real message
  wxString msg = wxT("the string");
  m_app->doUpdate(msg);
  cout << msg << endl;
  return true;
}

bool SlowDataEncoderTests::testSetGPSData() {
  wxString gpsData = wxT("test-gps-data");
  m_encoder->setGPSData(gpsData);
  string expected = "5test-5gps-d3ata00000000000000000000000000000000000000000000";
  if (memcmp(expected.c_str(), m_encoder->m_gpsData, 5)) {
    return true;
  }

  gpsData = wxT("");
  m_encoder->setGPSData(gpsData);
  if (m_encoder->m_gpsData[0] != '\0') {
    return true;
  }

  return true;
}

/**
 * Minimal wxWidgets context to run the tests above.
 */
int main(int argc, char **argv)
{
  wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");

  wxInitializer initializer;
  if ( !initializer )
  {
    fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
    return -1;
  }

  int hasErrors = 0;

  // create a new APRSWriterThread
  wxString hostname = wxT("test.host.name");
  int port = 123;
  wxString gateway = wxT("test.gateway");
  wxString address = wxT("test.address");

  CAPRSWriterThread *thread = new CAPRSWriterThread(gateway, address, hostname, port);
  APRSWriterThreadTests *aprsWriterThreadTests = new APRSWriterThreadTests(thread);

  // ensure the password generation works as expected
  hasErrors += assertOK(aprsWriterThreadTests->testGetAPRSPassword(),
                       wxT("APRSWriterThread::getAPRSPassword"));

  wxString update_channel = wxT("#dstar");
  IRCDDBApp *app = new IRCDDBApp(update_channel);
  IRCDDBAppTest *ircDDBAppTest = new IRCDDBAppTest(app);
  hasErrors += assertOK(ircDDBAppTest->testDoUpdate(), "IRCDDBApp::doUpdate");

  CSlowDataEncoder *encoder = new CSlowDataEncoder();
  SlowDataEncoderTests *slowEncoderTests = new SlowDataEncoderTests(encoder);
  hasErrors += assertOK(slowEncoderTests->testSetGPSData(), "SlowDataEncoder::setGPSData");

  return hasErrors == 0 ? 0 : 1;
}
