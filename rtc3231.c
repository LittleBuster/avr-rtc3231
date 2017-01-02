/* DS3231 Real Time Clock AVR Lirary
 *
 * Copyright (C) 2016-2017 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by Adafruit Industries. MIT license.
 */

#include "rtc3231.h"
#include "i2c.h"

static unsigned char bcd (unsigned char data)
{
	unsigned char bc;

	bc = ((((data & (1 << 6)) | (data & (1 << 5)) | (data & (1 << 4)))*0x0A) >> 4)
	+ ((data & (1 << 3))|(data & (1 << 2))|(data & (1 << 1))|(data & 0x01));

  return bc;
}

static unsigned char bin(unsigned char dec)
{
	char bcd;
	char n, dig, num, count;

	num = dec;
	count = 0;
	bcd = 0;

	for (n = 0; n < 4; n++) {
		dig = num % 10;
		num = num / 10;
		bcd = (dig << count) | bcd;
		count += 4;
	}
	return bcd;
}


void rtc3231_init(void)
{
	i2c_start_condition();
	i2c_send_byte(RTC_WADDR);
	i2c_send_byte(0x0E);
	i2c_send_byte(0x20);
	i2c_send_byte(0x08);
	i2c_stop_condition();
}

void rtc3231_read_time(struct rtc_time *time)
{
	i2c_start_condition();
	i2c_send_byte(RTC_WADDR);
	i2c_send_byte(0x00);
	i2c_stop_condition();

	i2c_start_condition();
	i2c_send_byte(RTC_RADDR);
	time->sec = bcd(i2c_recv_byte());
	time->min = bcd(i2c_recv_byte());
	time->hour = bcd(i2c_recv_byte());
	i2c_stop_condition();
}

void rtc3231_read_date(struct rtc_date *date)
{
	i2c_start_condition();
	i2c_send_byte(RTC_WADDR);
	i2c_send_byte(0x00);
	i2c_stop_condition();

	i2c_start_condition();
	i2c_send_byte(RTC_RADDR);
	bcd(i2c_recv_byte());
	bcd(i2c_recv_byte());
	bcd(i2c_recv_byte());

	date->wday = bcd(i2c_recv_byte());
	date->day = bcd(i2c_recv_byte());
	date->month = bcd(i2c_recv_byte());
	date->year = bcd(i2c_recv_last_byte());
	i2c_stop_condition();
}

void rtc3231_read_datetime(struct rtc_time *time, struct rtc_date *date)
{
	i2c_start_condition();
	i2c_send_byte(RTC_WADDR);
	i2c_send_byte(0x00);
	i2c_stop_condition();

	i2c_start_condition();
	i2c_send_byte(RTC_RADDR);
	time->sec = bcd(i2c_recv_byte());
	time->min = bcd(i2c_recv_byte());
	time->hour = bcd(i2c_recv_byte());

	date->wday = bcd(i2c_recv_byte());
	date->day = bcd(i2c_recv_byte());
	date->month = bcd(i2c_recv_byte());
	date->year = bcd(i2c_recv_last_byte());
	i2c_stop_condition();
}

void rtc3231_write_time(struct rtc_time *time)
{
    i2c_start_condition();
    i2c_send_byte(RTC_WADDR);
    i2c_send_byte(0x00);
    i2c_send_byte(bin(time->sec));
	i2c_send_byte(bin(time->min));
    i2c_send_byte(bin(time->hour));
    i2c_stop_condition();
}

void rtc3231_write_date(struct rtc_date *date)
{
	i2c_start_condition();
    i2c_send_byte(RTC_WADDR);
    i2c_send_byte(0x03);
    i2c_send_byte(bin(date->wday));
    i2c_send_byte(bin(date->day));
	i2c_send_byte(bin(date->month));
	i2c_send_byte(bin(date->year));
    i2c_stop_condition();
}
