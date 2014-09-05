/*
  EEPROM.h - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef I2CEEPROM_h
#define I2CEEPROM_h

#include <inttypes.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif

#define I2CEEPROM_Address 0x50
#define I2CEEPROM_PAGES 0x200
#define I2CEEPROM_BYTES_PER_PAGE 0x40


class I2CEEPROMClass
{
  public:
    void writeByte( unsigned int eeaddress, byte data ) ;
    void writePage( unsigned int eeaddresspage, byte* data, byte length );
    byte readByte( unsigned int eeaddress );
    void readBuffer( unsigned int eeaddress, byte *buffer, int length ) ;
};

extern I2CEEPROMClass i2ceeprom;

#endif

