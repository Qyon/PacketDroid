/*
 *      multimon.h -- Monitor for many different modulation formats
 *
 *      Copyright (C) 1996  
 *          Thomas Sailer (sailer@ife.ee.ethz.ch, hb9jnx@hb9w.che.eu)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ---------------------------------------------------------------------- */

#ifndef _MULTIMON_H
#define _MULTIMON_H

#include <android/log.h>
#define LOG_TAG "MultimonDroid"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)


/* ---------------------------------------------------------------------- */

extern const float costabf[0x400];
#define COS(x) costabf[(((x)>>6)&0x3ffu)]
#define SIN(x) COS((x)+0xc000)

/* ---------------------------------------------------------------------- */

struct demod_state {
	const struct demod_param *dem_par;
	union {
		struct l2_state_hdlc {
			unsigned char rxbuf[512];
			unsigned char *rxptr;
			unsigned int rxstate;
			unsigned int rxbitstream;
			unsigned int rxbitbuf;
		} hdlc;

		struct l2_state_pocsag {
			unsigned long rx_data;
			struct l2_pocsag_rx {
				unsigned char rx_sync;
				unsigned char rx_word;
				unsigned char rx_bit;
				char func;
				unsigned long adr;
				unsigned char buffer[128];
				unsigned int numnibbles;
			} rx[2];
		} pocsag;
	} l2;
	union {
		struct l1_state_poc5 {
			unsigned int dcd_shreg;
			unsigned int sphase;
			unsigned int subsamp;
		} poc5;

		struct l1_state_poc12 {
			unsigned int dcd_shreg;
			unsigned int sphase;
			unsigned int subsamp;
		} poc12;

		struct l1_state_poc24 {
			unsigned int dcd_shreg;
			unsigned int sphase;
		} poc24;

		struct l1_state_afsk12 {
			unsigned int dcd_shreg;
			unsigned int sphase;
			unsigned int lasts;
			unsigned int subsamp;
		} afsk12;

		struct l1_state_afsk24 {
			unsigned int dcd_shreg;
			unsigned int sphase;
			unsigned int lasts;
		} afsk24;

		struct l1_state_hapn48 {
			unsigned int shreg;
			unsigned int sphase;
			float lvllo, lvlhi;
		} hapn48;

		struct l1_state_fsk96 {
			unsigned int dcd_shreg;
			unsigned int sphase;
			unsigned int descram;
		} fsk96;

		struct l1_state_dtmf {
			unsigned int ph[8];
			float energy[4];
			float tenergy[4][16];
			int blkcount;
			int lastch;
		} dtmf;

		struct l1_state_zvei {
			unsigned int ph[16];
			float energy[4];
			float tenergy[4][32];
			int blkcount;
			int lastch;
		} zvei;

/* 		struct l1_state_scope { */
/* 			int datalen; */
/* 			int dispnum; */
/* 			float data[512]; */
/* 		} scope; */
	} l1;
};

struct demod_param {
	const char *name;
	unsigned int samplerate;
	unsigned int overlap;
	void (*init)(struct demod_state *s);
	void (*demod)(struct demod_state *s, float *buffer, int length);
};

/* ---------------------------------------------------------------------- */

extern const struct demod_param demod_afsk1200;

//extern const struct demod_param demod_scope;

#define ALL_DEMOD &demod_afsk1200



/* ---------------------------------------------------------------------- */

void verbprintf(int verb_level, const char *fmt, ...);

void hdlc_init(struct demod_state *s);
void hdlc_rxbit(struct demod_state *s, int bit);

void pocsag_init(struct demod_state *s);
void pocsag_rxbit(struct demod_state *s, int bit);


// TODO this can't be a constant. FIXME
#define NAMED_PIPE "/data/data/com.jazzido.PacketDroid/pipe"

/* ---------------------------------------------------------------------- */
#endif /* _MULTIMON_H */
