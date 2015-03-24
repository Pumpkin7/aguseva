/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef FILE_HH_3801739750__H_
#define FILE_HH_3801739750__H_


#include <sstream>
#include "boost/any.hpp"
#include "Specific.hh"
#include "Encoder.hh"
#include "Decoder.hh"

namespace c {
struct file {
    std::string filepath;
    std::string filename;
    int64_t filesize;
    file() :
        filepath(std::string()),
        filename(std::string()),
        filesize(int64_t())
        { }
};

}
namespace avro {
template<> struct codec_traits<c::file> {
    static void encode(Encoder& e, const c::file& v) {
		avro::encode(e, v.filepath);
        avro::encode(e, v.filename);
        avro::encode(e, v.filesize);

    }
    static void decode(Decoder& d, c::file& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.filepath);
                    break;
                case 1:
                    avro::decode(d, v.filename);
                    break;
                case 2:
                    avro::decode(d, v.filesize);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.filepath);
            avro::decode(d, v.filename);
            avro::decode(d, v.filesize);
        }
    }
};

}
#endif
