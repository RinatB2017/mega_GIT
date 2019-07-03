/*
Tag12.h
Trust Packet

Copyright (c) 2013 - 2018 Jason Lee @ calccrypto at gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef __TAG12__
#define __TAG12__

#include "Packet.h"

namespace OpenPGP {
    namespace Packet {

        // 5.10. Trust Packet (Tag 12)
        //
        //    The Trust packet is used only within keyrings and is not normally
        //    exported. Trust packets contain data that record the user’s
        //    specifications of which key holders are trustworthy introducers,
        //    along with other information that implementing software uses for
        //    trust information. The format of Trust packets is defined by a given
        //    implementation.
        //
        //    Trust packets SHOULD NOT be emitted to output streams that are
        //    transferred to other users, and they SHOULD be ignored on any input
        //    other than local keyring files.

        class Tag12 : public Tag {
            private:
                std::string trust;

            public:
                typedef std::shared_ptr <Packet::Tag12> Ptr;

                Tag12();
                Tag12(const Tag12 & copy);
                Tag12(const std::string & data);
                Tag12(std::istream & stream);
                void read(const std::string & data);
                std::string show(const std::size_t indents = 0, const std::size_t indent_size = 4) const;
                std::string raw() const;

                std::string get_trust() const;

                void set_trust(const std::string & t);

                Tag::Ptr clone() const;
        };
    }
}

#endif
