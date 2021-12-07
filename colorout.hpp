/* colorout.hpp
 * (c) 2021 Daniel S. Roche, USNA
 *
 * The class colorout below provides the functionality of
 * cout or cerr (depending on the first constructor argument being
 * 1 or 2, respectively). The only difference is that the output
 * is printed in the color corresponding to the second argument.
 * You use it by first calling the constructor to make a new object:
colorout out(1, 'r'); // r is for red
 * and then you can use that just like cout:
out << "This is " << "all red!\n";
 * Note that cout still prints in the default color.
 *
 * The optional third argument 'block' is a pointer to a bool that,
 * if non-NULL and the bool pointed to is true, then it will block
 * this output stream entirely. (Useful for supressing later output
 * when an error has occurred.)
 */

#ifndef COLOROUT_HPP
#define COLOROUT_HPP

#include <unistd.h> // for isatty
#include <cstdlib>  // for exit
#include <iostream>
#include <fstream> // for filebuf
#include <string>

class colorbuf : public std::streambuf {
 private:
  std::streambuf *dest;
  std::filebuf fileout; // only used in case of redirect
  bool tty;
  char magic[7];
  const bool* block;
 public:
  colorbuf (int filedes, char color, const bool* b = NULL)
    :block(b)
  {
    if (filedes == 1) dest = std::cout.rdbuf();
    else if (filedes == 2) dest = std::cerr.rdbuf();
    else {
      std::cerr << "Illegal file descriptor in colorbuf constructor" << std::endl;
      exit(11);
    }
    tty = isatty(filedes);

    int colorcode;
    switch(color) {
      case 'b': colorcode = 30; break; // black
      case 'r': colorcode = 31; break; // red
      case 'g': colorcode = 32; break; // green
      case 'y': colorcode = 33; break; // yellow
      case 'u': colorcode = 34; break; // blue
      case 'w': colorcode = 37; break; // white
      default:
        std::cerr << "Illegal color for colorbuf" << std::endl;
        exit(12);
    }

    magic[0] = '\033';
    magic[1] = '[';
    magic[2] = colorcode / 10 + '0';
    magic[3] = colorcode % 10 + '0';
    magic[4] = ';';
    magic[5] = '1';
    magic[6] = 'm';
  }

  // redirects the output of this buffer to the given file
  bool redirect(std::string fname) {
    if (! fileout.open(fname.c_str(), std::ios_base::out | std::ios_base::trunc))
      return false;
    tty = false;
    dest = &fileout;
    return true;
  }

 protected:
  virtual int overflow(int ch) {
    if (block && *block) return 1;
    if (tty) dest->sputn(magic, 7);
    dest->sputc(ch);
    if (tty) dest->sputn("\033[0m", 4);
    return 1;
  }
};

class colorout : public std::ostream {
 private:
  colorbuf buf;
 public:
  colorout(int fd, int cc, const bool* block = NULL)
    :std::ostream(&buf), buf(fd,cc,block)
    { }

  // redirects the output of this stream to the given file
  bool redirect(std::string fname) {
    return buf.redirect(fname.c_str());
  }
};

// This forces flex to read one character at a time ONLY.
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
  { \
    int c = getc(yyin); \
    if ((max_size) < 1) (result) = 0; \
    else if (c == EOF) (result) = YY_NULL; \
    else { \
      (buf)[0] = c; \
      (result) = 1; \
    } \
  }

#endif // COLOROUT_HPP
