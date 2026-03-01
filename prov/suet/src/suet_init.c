/*
 * Copyright (c) 2026, ETH Zurich. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 #include <ofi.h>
#include <ofi_prov.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct fi_provider suet_prov;

static int suet_getinfo(uint32_t version, const char *node, const char *service,
                        uint64_t flags, const struct fi_info *hints,
                        struct fi_info **info)
{
    struct fi_info *my_info;

    // Reject if hints prov_name is given and doesn't match 'suet'
    if (hints && hints->fabric_attr && hints->fabric_attr->prov_name) {
        if (strcmp(hints->fabric_attr->prov_name, "suet") != 0 && 
            !strstr(hints->fabric_attr->prov_name, "suet")) {
            return -FI_ENODATA;
        }
    }

    // Reject if any specific endpoint type is requested
    if (hints && hints->ep_attr && hints->ep_attr->type != FI_EP_UNSPEC) {
        return -FI_ENODATA;
    }

    // Reject if any caps are requested
    if (hints && hints->caps) {
        return -FI_ENODATA;
    }

    my_info = fi_allocinfo();
    if (!my_info)
        return -FI_ENOMEM;

    my_info->ep_attr->type = FI_EP_UNSPEC;
    my_info->caps = 0;

    my_info->fabric_attr->prov_version = OFI_VERSION_DEF_PROV;
    my_info->fabric_attr->name = strdup("suet");
    my_info->domain_attr->name = strdup("suet");

    *info = my_info;
    return 0;
}

static int suet_fabric(struct fi_fabric_attr *attr,
                       struct fid_fabric **fabric, void *context)
{
    return -FI_ENOSYS;
}

static void suet_fini(void)
{
}

struct fi_provider suet_prov = {
        .name = "suet",
        .version = OFI_VERSION_DEF_PROV,
        .fi_version = OFI_VERSION_LATEST,
        .getinfo = suet_getinfo,
        .fabric = suet_fabric,
        .cleanup = suet_fini
};

struct fi_provider * fi_suet_ini(void)
{
        return &suet_prov;
}
