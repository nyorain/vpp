/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

template<std::size_t D, class P> using SquareMat = Mat<D, D, P>;

template<class P> using Mat2 = SquareMat<2,P>;
template<class P> using Mat3 = SquareMat<3,P>;
template<class P> using Mat4 = SquareMat<4,P>;
template<class P> using Mat5 = SquareMat<5,P>;

template<class P> using Mat23 = Mat<2,3,P>;
template<class P> using Mat24 = Mat<2,4,P>;
template<class P> using Mat25 = Mat<2,5,P>;

template<class P> using Mat32 = Mat<3,2,P>;
template<class P> using Mat34 = Mat<3,4,P>;
template<class P> using Mat35 = Mat<3,5,P>;

template<class P> using Mat42 = Mat<4,2,P>;
template<class P> using Mat43 = Mat<4,3,P>;
template<class P> using Mat45 = Mat<4,5,P>;

template<class P> using Mat52 = Mat<5,2,P>;
template<class P> using Mat53 = Mat<5,3,P>;
template<class P> using Mat54 = Mat<5,4,P>;

typedef Mat2<float> Mat2f;
typedef Mat2<unsigned int> Mat2ui;
typedef Mat2<int> Mat2i;
typedef Mat2<double> Mat2d;
typedef Mat2<char> Mat2c;
typedef Mat2<unsigned char> Mat2uc;
typedef Mat2<long> Mat2l;
typedef Mat2<unsigned long> Mat2ul;
typedef Mat2<short> Mat2s;
typedef Mat2<unsigned short> Mat2us;

typedef Mat3<float> Mat3f;
typedef Mat3<unsigned int> Mat3ui;
typedef Mat3<int> Mat3i;
typedef Mat3<double> Mat3d;
typedef Mat3<char> Mat3c;
typedef Mat3<unsigned char> Mat3uc;
typedef Mat3<long> Mat3l;
typedef Mat3<unsigned long> Mat3ul;
typedef Mat3<short> Mat3s;
typedef Mat3<unsigned short> Mat3us;

typedef Mat4<float> Mat4f;
typedef Mat4<unsigned int> Mat4ui;
typedef Mat4<int> Mat4i;
typedef Mat4<double> Mat4d;
typedef Mat4<char> Mat4c;
typedef Mat4<unsigned char> Mat4uc;
typedef Mat4<long> Mat4l;
typedef Mat4<unsigned long> Mat4ul;
typedef Mat4<short> Mat4s;
typedef Mat4<unsigned short> Mat4us;

typedef Mat5<float> Mat5f;
typedef Mat5<unsigned int> Mat5ui;
typedef Mat5<int> Mat5i;
typedef Mat5<double> Mat5d;
typedef Mat5<char> Mat5c;
typedef Mat5<unsigned char> Mat5uc;
typedef Mat5<long> Mat5l;
typedef Mat5<unsigned long> Mat5ul;
typedef Mat5<short> Mat5s;
typedef Mat5<unsigned short> Mat5us;


//
typedef Mat23<float> Mat23f;
typedef Mat23<unsigned int> Mat23ui;
typedef Mat23<int> Mat23i;
typedef Mat23<double> Mat23d;
typedef Mat23<char> Mat23c;
typedef Mat23<unsigned char> Mat23uc;
typedef Mat23<long> Mat23l;
typedef Mat23<unsigned long> Mat23ul;
typedef Mat23<short> Mat23s;
typedef Mat23<unsigned short> Mat23us;

typedef Mat24<float> Mat24f;
typedef Mat24<unsigned int> Mat24ui;
typedef Mat24<int> Mat24i;
typedef Mat24<double> Mat24d;
typedef Mat24<char> Mat24c;
typedef Mat24<unsigned char> Mat24uc;
typedef Mat24<long> Mat24l;
typedef Mat24<unsigned long> Mat24ul;
typedef Mat24<short> Mat24s;
typedef Mat24<unsigned short> Mat24us;

typedef Mat25<float> Mat25f;
typedef Mat25<unsigned int> Mat25ui;
typedef Mat25<int> Mat25i;
typedef Mat25<double> Mat25d;
typedef Mat25<char> Mat25c;
typedef Mat25<unsigned char> Mat25uc;
typedef Mat25<long> Mat25l;
typedef Mat25<unsigned long> Mat25ul;
typedef Mat25<short> Mat25s;
typedef Mat25<unsigned short> Mat25us;

typedef Mat32<float> Mat32f;
typedef Mat32<unsigned int> Mat32ui;
typedef Mat32<int> Mat32i;
typedef Mat32<double> Mat32d;
typedef Mat32<char> Mat32c;
typedef Mat32<unsigned char> Mat32uc;
typedef Mat32<long> Mat32l;
typedef Mat32<unsigned long> Mat32ul;
typedef Mat32<short> Mat32s;
typedef Mat32<unsigned short> Mat32us;

typedef Mat34<float> Mat34f;
typedef Mat34<unsigned int> Mat34ui;
typedef Mat34<int> Mat34i;
typedef Mat34<double> Mat34d;
typedef Mat34<char> Mat34c;
typedef Mat34<unsigned char> Mat34uc;
typedef Mat34<long> Mat34l;
typedef Mat34<unsigned long> Mat34ul;
typedef Mat34<short> Mat34s;
typedef Mat34<unsigned short> Mat34us;

typedef Mat35<float> Mat35f;
typedef Mat35<unsigned int> Mat35ui;
typedef Mat35<int> Mat35i;
typedef Mat35<double> Mat35d;
typedef Mat35<char> Mat35c;
typedef Mat35<unsigned char> Mat35uc;
typedef Mat35<long> Mat35l;
typedef Mat35<unsigned long> Mat35ul;
typedef Mat35<short> Mat35s;
typedef Mat35<unsigned short> Mat35us;

typedef Mat42<float> Mat42f;
typedef Mat42<unsigned int> Mat42ui;
typedef Mat42<int> Mat42i;
typedef Mat42<double> Mat42d;
typedef Mat42<char> Mat42c;
typedef Mat42<unsigned char> Mat42uc;
typedef Mat42<long> Mat42l;
typedef Mat42<unsigned long> Mat42ul;
typedef Mat42<short> Mat42s;
typedef Mat42<unsigned short> Mat42us;

typedef Mat43<float> Mat43f;
typedef Mat43<unsigned int> Mat43ui;
typedef Mat43<int> Mat43i;
typedef Mat43<double> Mat43d;
typedef Mat43<char> Mat43c;
typedef Mat43<unsigned char> Mat43uc;
typedef Mat43<long> Mat43l;
typedef Mat43<unsigned long> Mat43ul;
typedef Mat43<short> Mat43s;
typedef Mat43<unsigned short> Mat43us;

typedef Mat45<float> Mat45f;
typedef Mat45<unsigned int> Mat45ui;
typedef Mat45<int> Mat45i;
typedef Mat45<double> Mat45d;
typedef Mat45<char> Mat45c;
typedef Mat45<unsigned char> Mat45uc;
typedef Mat45<long> Mat45l;
typedef Mat45<unsigned long> Mat45ul;
typedef Mat45<short> Mat45s;
typedef Mat45<unsigned short> Mat45us;

typedef Mat52<float> Mat52f;
typedef Mat52<unsigned int> Mat52ui;
typedef Mat52<int> Mat52i;
typedef Mat52<double> Mat52d;
typedef Mat52<char> Mat52c;
typedef Mat52<unsigned char> Mat52uc;
typedef Mat52<long> Mat52l;
typedef Mat52<unsigned long> Mat52ul;
typedef Mat52<short> Mat52s;
typedef Mat52<unsigned short> Mat52us;

typedef Mat53<float> Mat53f;
typedef Mat53<unsigned int> Mat53ui;
typedef Mat53<int> Mat53i;
typedef Mat53<double> Mat53d;
typedef Mat53<char> Mat53c;
typedef Mat53<unsigned char> Mat53uc;
typedef Mat53<long> Mat53l;
typedef Mat53<unsigned long> Mat53ul;
typedef Mat53<short> Mat53s;
typedef Mat53<unsigned short> Mat53us;

typedef Mat54<float> Mat54f;
typedef Mat54<unsigned int> Mat54ui;
typedef Mat54<int> Mat54i;
typedef Mat54<double> Mat54d;
typedef Mat54<char> Mat54c;
typedef Mat54<unsigned char> Mat54uc;
typedef Mat54<long> Mat54l;
typedef Mat54<unsigned long> Mat54ul;
typedef Mat54<short> Mat54s;
typedef Mat54<unsigned short> Mat54us;
