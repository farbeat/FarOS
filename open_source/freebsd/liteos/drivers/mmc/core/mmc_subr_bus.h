/*-
 * Copyright (c) 1997,1998,2003 Doug Rabson
 * All rights reserved.
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef DEV_MMC_SUBR_BUS_H
#define DEV_MMC_SUBR_BUS_H

#include "mmc_type.h"
#include "dpal.h"

#ifdef __OHOS__
static inline void *
mmcdev_get_private_data(device_t dev)
{
	return device_get_ivars(dev);
}

static inline void
mmcdev_set_private_data(device_t dev, void *data)
{
	device_set_ivars(dev, data);
}
#else /* LiteOS */
static inline void *
mmcdev_get_private_data(device_t dev)
{
	return (dev ? dev->driver_data : NULL);
}

static inline void
mmcdev_set_private_data(device_t dev, void *data)
{
	if (dev == NULL)
		return;
	dev->driver_data = data;
}

static inline device_t
device_get_parent(device_t dev)
{
	return (dev ? dev->platform_data : NULL);
}

static inline void
device_set_parent(device_t dev, device_t parent)
{
	if (dev == NULL)
		return;
	dev->platform_data = parent;
}

static const char unknown_string[] = { "unknown" };
static inline const char *
device_get_nameunit(device_t dev)
{
	if (dev && dev->name)
		return (dev->name);

	return (unknown_string);
}

static inline void
device_set_nameunit(device_t dev, const char *name)
{
	if (dev == NULL)
		return;
	dev->name = name;
}

#endif /* __OHOS__ */

#ifdef __FREERTOS__
#define	device_printf(dev, fmt, args...) do {	\
    dprintf("%s: ", device_get_nameunit(dev));	\
    dprintf(fmt, ##args); \
} while (0)
#else
#define	device_printf(dev, fmt,...) \
	dprintf("%s: " fmt, device_get_nameunit(dev), ## __VA_ARGS__)
#endif

typedef struct {
	void *softc;		/* Current driver's variables */
	void *ivars;		/* Instance variables */
	void *ops;			/* Driver's operators */
	void *child;		/* Only used for the slot and the mmc bus layer */
} mmcdev_priv;

static inline void *
get_mmcbus_ops(device_t dev)
{
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv == NULL)
		return (NULL);

	return (priv->ops);
}

static inline void
mmcdev_set_ivars(device_t dev, void *ivars)
{
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv == NULL)
		return;
	priv->ivars = ivars;
}

static inline void *
mmcdev_get_ivars(device_t dev)
{
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv == NULL)
		return (NULL);
	return (priv->ivars);
}

static inline void *
mmcdev_get_softc(device_t dev)
{
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv == NULL)
		return (NULL);
	return (priv->softc);
}

static inline void
mmcdev_set_softc(device_t dev, void *softc)
{
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv == NULL)
		return;
	priv->softc = softc;
}

static inline device_t
mmcdev_get_child(device_t dev)
{
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv == NULL)
		return (NULL);
	return (priv->child);
}

static inline void
mmcdev_set_child(device_t dev, device_t child)
{
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv == NULL)
		return;
	priv->child = child;
}

static inline int
mmcdev_priv_create(device_t dev, void *ops, void *softc, void *ivars)
{
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv == NULL) {
		priv = zalloc(sizeof(mmcdev_priv));
		if (priv == NULL) {
			return (-ENOMEM);
		}
	}

	priv->softc = softc;
	priv->ivars = ivars;
	priv->ops = ops;
	mmcdev_set_private_data(dev, priv);
	return (ENOERR);
}

static inline void
mmcdev_priv_destroy(device_t dev)
{
	if (dev == NULL)
		return;
	mmcdev_priv *priv = (mmcdev_priv *)mmcdev_get_private_data(dev);
	if (priv != NULL) {
		free(priv);
		mmcdev_set_private_data(dev, NULL);
	}
}

#define device_set_ivars mmcdev_set_ivars
#define device_get_ivars mmcdev_get_ivars
#define device_set_softc mmcdev_set_softc
#define device_get_softc mmcdev_get_softc

#endif /* DEV_MMC_SUBR_BUS_H */
