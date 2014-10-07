#ifndef FAST_Q_H
#define FAST_Q_H

#include <string>

class FastQ
{

public:
  FastQ(const std::string& file_name);
  ~FastQ();

private:
  FastQ() = delete;                              // don't use the default constructor
  FastQ(const FastQ& fastq) = delete;            // don't use copy constructor
  FastQ& operator=(const FastQ& fastq) = delete; // don't use operator equals

  size_t m_length;
  std::string m_file_name;

};

#endif // end namespace FAST_Q_H
