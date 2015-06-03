Cli::Cli(class Logger *logger) :
  m_logger(logger),
  m_ready(false)
{
  m_inputbuffer[0] = '\0';
  this->setup();
}

bool Cli::is_available() { return m_ready; }

void Cli::setup()
{
  Serial.begin(9600);
  Serial.println("Enter percentage for blink ratio");
}

void Cli::refresh() {
  bool modified = false;
  while (Serial.available()) {
    char ch = Serial.read();
    modified = true;
    if (ch == '\r') {
      m_ready = true;
    } else if (m_nextkey_i < sizeof(m_inputbuffer)-1) {
      m_inputbuffer[m_nextkey_i++] = ch;
      m_inputbuffer[m_nextkey_i] = '\0';
    }
  }
  if (modified) {
    m_logger->debug("Got so far:");
    m_logger->debug(m_inputbuffer);
  }
}

int Cli::getNumber()
{
  int i = 0;
  int n = 0;
  while (i<sizeof(m_inputbuffer)-1) {
    int ch = m_inputbuffer[i];
    if (ch>='0' && ch<='9') {
       n = 10*n + ch-'0';
       i++;
    }
    else
      break;
  }
  m_nextkey_i = 0;
  m_ready = false;
  if (m_inputbuffer[i]=='\0') {
    char buffer[50];
    sprintf(buffer,"New INT value: %d",n);
    Serial.println(buffer);
    return n;
  }
  return -1;
}

