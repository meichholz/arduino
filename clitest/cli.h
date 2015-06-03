#ifndef _H_CLI
#define _H_CLI

class Cli {
  public:

  Cli(class Logger *logger);
  int getNumber();
  void setup();
  void refresh();
  bool is_available();
  virtual ~Cli() {};

  private:
    char m_inputbuffer[100];
    bool m_ready;
    int  m_nextkey_i;
    class Logger *m_logger;
};
#endif
