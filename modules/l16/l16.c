/**
 * @file l16.c  16-bit linear codec
 *
 * Copyright (C) 2010 Creytiv.com
 */
#include <re.h>
#include <baresip.h>


enum {NR_CODECS = 8};


struct aucodec_st {
	struct aucodec *ac;  /* inheritance */
};


static int encode(struct auenc_state *st, uint8_t *buf, size_t *len,
		  const int16_t *sampv, size_t sampc)
{
	int16_t *p = (void *)buf;

	(void)st;

	if (sampc*2 > *len)
		return ENOMEM;

	*len = sampc*2;

	while (sampc--)
		*p++ = htons(*sampv++);

	return 0;
}


static int decode(struct audec_state *st, int16_t *sampv, size_t *sampc,
		  const uint8_t *buf, size_t len)
{
	int16_t *p = (void *)buf;

	(void)st;

	if (len/2 > *sampc)
		return ENOMEM;

	*sampc = len/2;

	while ((len -= 2))
		*sampv++ = ntohs(*p++);

	return 0;
}


/* See RFC 3551 */
static struct aucodec l16v[NR_CODECS] = {
	{LE_INIT, "10", "L16", 44100, 2, 0, 0, encode, 0, decode, 0, 0, 0},
	{LE_INIT,    0, "L16", 32000, 2, 0, 0, encode, 0, decode, 0, 0, 0},
	{LE_INIT,    0, "L16", 16000, 2, 0, 0, encode, 0, decode, 0, 0, 0},
	{LE_INIT,    0, "L16",  8000, 2, 0, 0, encode, 0, decode, 0, 0, 0},
	{LE_INIT, "11", "L16", 44100, 1, 0, 0, encode, 0, decode, 0, 0, 0},
	{LE_INIT,    0, "L16", 32000, 1, 0, 0, encode, 0, decode, 0, 0, 0},
	{LE_INIT,    0, "L16", 16000, 1, 0, 0, encode, 0, decode, 0, 0, 0},
	{LE_INIT,    0, "L16",  8000, 1, 0, 0, encode, 0, decode, 0, 0, 0},
};


static int module_init(void)
{
	size_t i;

	for (i=0; i<NR_CODECS; i++)
		aucodec_register(&l16v[i]);

	return 0;
}


static int module_close(void)
{
	size_t i;

	for (i=0; i<NR_CODECS; i++)
		aucodec_unregister(&l16v[i]);

	return 0;
}


EXPORT_SYM const struct mod_export DECL_EXPORTS(l16) = {
	"l16",
	"codec",
	module_init,
	module_close
};
