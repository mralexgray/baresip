/**
 * @file ilbc.c  Internet Low Bit Rate Codec (iLBC) audio codec
 *
 * Copyright (C) 2010 Creytiv.com
 */
#include <re.h>
#include <baresip.h>
#include <iLBC_define.h>
#include <iLBC_decode.h>
#include <iLBC_encode.h>


#define DEBUG_MODULE "ilbc"
#define DEBUG_LEVEL 5
#include <re_dbg.h>


/*
 * This module implements the iLBC audio codec as defined in:
 *
 *     RFC 3951  Internet Low Bit Rate Codec (iLBC)
 *     RFC 3952  RTP Payload Format for iLBC Speech
 *
 * The iLBC source code is not included here, but can be downloaded from
 * http://ilbcfreeware.org/
 *
 * You can also use the source distributed by the Freeswitch project,
 * see www.freeswitch.org, and then freeswitch/libs/codec/ilbc.
 * Or you can look in the asterisk source code ...
 *
 *   mode=20  15.20 kbit/s  160samp  38bytes
 *   mode=30  13.33 kbit/s  240samp  50bytes
 */

enum {
	DEFAULT_MODE = 20, /* 20ms or 30ms */
	USE_ENHANCER = 1
};

struct auenc_state {
	iLBC_Enc_Inst_t enc;
	int mode_enc;
    uint32_t enc_bytes;
};

struct audec_state {
	iLBC_Dec_Inst_t dec;
    int mode_dec;
	uint32_t dec_nsamp;
	size_t dec_bytes;
};

static char ilbc_fmtp[32];


static void encode_destructor(void *arg)
{
	struct auenc_state *st = arg;
 	mem_deref(st);	
}


static void decode_destructor(void *arg)
{
	struct audec_state *st = arg;

	mem_deref(st);
}

static int encode_update(struct auenc_state **aesp, const struct aucodec *ac,
			 struct auenc_param *prm, const char *fmtp)
{
	struct auenc_state *st;
	int err = 0;
    struct pl mode;
	(void)ac;
	(void)prm;
	(void)fmtp;

	if (!aesp)
		return EINVAL;
	if (*aesp)
		return 0;

	st = mem_zalloc(sizeof(*st), encode_destructor);
	if (!st)
		return ENOMEM;

    if (st->mode_enc == pl_u32(&mode))
    return;

    (void)re_printf("set iLBC encodeder mode %dms\n", pl_u32(&mode));

    st->mode_enc = pl_u32(&mode);

    switch (pl_u32(&mode)) {

        case 20:
            st->enc_bytes = NO_OF_BYTES_20MS;
            break;

        case 30:
            st->enc_bytes = NO_OF_BYTES_30MS;
            break;

        default:
            DEBUG_WARNING("unknown encoder mode %d\n", pl_u32(&mode));
            return;
    }

    st->enc_bytes = initEncode(&st->enc, pl_u32(&mode));
    if (!&st->enc) {
		err = EPROTO;
		goto out;
	}

 out:
	if (err)
        encode_destructor(st);	
	else
		*aesp = st;

	return err;
}


static int decode_update(struct audec_state **adsp,
			 const struct aucodec *ac, const char *fmtp)
{
	struct audec_state *st;
	struct pl mode;
    int err = 0;
	(void)ac;
	(void)fmtp;

	if (!adsp)
		return EINVAL;
	if (*adsp)
		return 0;

	st = mem_zalloc(sizeof(*st), encode_destructor);
	if (!st)
		return ENOMEM;
    
    (void)re_printf("set iLBC decoder mode %dms\n", pl_u32(&mode));

    st->mode_dec    = pl_u32(&mode); 

    switch (pl_u32(&mode)) {

        case 20:
            st->dec_nsamp = BLOCKL_20MS;
            break;

        case 30:
            st->dec_nsamp = BLOCKL_30MS;
            break;

        default:
            DEFAULT_BITRATEEBUG_WARNING("unknown decoder mode %d\n", mode);
            return;
    }

    st->dec_nsamp = initDecode(&st->dec, pl_u32(&mode), USE_ENHANCER);

    if (!&st->dec) {
		err = EPROTO;
		goto out;
	}

 out:
	if (err)
        decode_destructor(st);	
	else
		*adsp = st;

	return err;
}


static int encode(struct auenc_state *st, uint8_t *buf,
                  size_t *len, const int16_t *sampv, size_t sampc)
{
    (void)sampc;
    (void)len;

    iLBC_encode(buf,        /* (o) encoded data bits iLBC */
		    sampv,          /* (o) speech vector to encode */
		    &st->enc);      /* (i/o) the general encoder state */

	return 0;
}

/* src=NULL means lost packet */
static int decode(struct audec_state *st, int16_t *sampv,
		  size_t *sampc, const uint8_t *buf, size_t len)
{
    const int mode = mbuf_get_left(sampv) ? 1 : 0;
    (void)sampc;
    (void)len;

    iLBC_decode(buf,        /* (o) decoded signal block */
		    sampv,          /* (i) encoded signal bits */
		    &st->dec,       /* (i/o) the decoder state structure */
		    mode);          /* (i) 0: bad packet, PLC, 1: normal */

	return 0;
}

static struct aucodec ilbc = {
	LE_INIT, NULL, "iLBC", 8000, 1, ilbc_fmtp,
	encode_update, encode, decode_update, decode, NULL, NULL, NULL
};

static int module_init(void)
{
	(void)re_snprintf(ilbc_fmtp, sizeof(ilbc_fmtp),
			  "mode=%d", DEFAULT_MODE);
	aucodec_register(&ilbc);
	return 0;
}


static int module_close(void)
{
	aucodec_unregister(&ilbc);	
	return 0;
}


/** Module exports */
EXPORT_SYM const struct mod_export DECL_EXPORTS(ilbc) = {
	"ilbc",
	"codec",
	module_init,
	module_close
};
