/*
 * The Python Imaging Library
 * $Id$
 *
 * Blend Veloland imSrc over imDst.
 *
 * See the README file for details on usage and redistribution.
 */


#include "Imaging.h"


Imaging
ImagingBlendVeloland(Imaging imDst, Imaging imSrc)
{
    Imaging imOut;
    int x, y;
    float dstR, dstG, dstB, dstA;
    float srcR, srcG, srcB, srcA;
    float outR, outG, outB, outA;

    /* Check arguments */
    if (!imDst || !imSrc ||
	strcmp(imDst->mode, "RGBA") ||
	imDst->type != IMAGING_TYPE_UINT8 ||
	imDst->bands != 4)
	return ImagingError_ModeError();
    if (strcmp(imDst->mode, imSrc->mode) ||
	imDst->type  != imSrc->type  ||
	imDst->bands != imSrc->bands ||
	imDst->xsize != imSrc->xsize ||
	imDst->ysize != imSrc->ysize)
	return ImagingError_Mismatch();

    imOut = ImagingNew(imDst->mode, imDst->xsize, imDst->ysize);
    if (!imOut)
	return NULL;

    ImagingCopyInfo(imOut, imDst);

    for (y = 0; y < imDst->ysize; y++) {

	UINT8* dst = (UINT8*) imDst->image[y];
	UINT8* src = (UINT8*) imSrc->image[y];
	UINT8* out = (UINT8*) imOut->image[y];

	for (x = 0; x < imDst->linesize; x += 4) {

	    if (dst[x + 0] == src[x + 0] &&
		dst[x + 1] == src[x + 1] &&
		dst[x + 2] == src[x + 2] &&
		dst[x + 3] == src[x + 3]) {

		out[x + 0] = dst[x + 0];
		out[x + 1] = dst[x + 1];
		out[x + 2] = dst[x + 2];
		out[x + 3] = dst[x + 3];

	    } else {

		dstR = dst[x + 0] / 255.0;
		dstG = dst[x + 1] / 255.0;
		dstB = dst[x + 2] / 255.0;
		dstA = dst[x + 3] / 255.0;

		srcR = src[x + 0] / 255.0;
		srcG = src[x + 1] / 255.0;
		srcB = src[x + 2] / 255.0;
		srcA = src[x + 3] / 255.0;

		if (dstA == 1.0) {
		    outR = srcR * srcA + dstR * (1.0 - srcA);
		    outG = srcG * srcA + dstG * (1.0 - srcA);
		    outB = srcB * srcA + dstB * (1.0 - srcA);
		    outA = 1.0;
		} else if (srcA == 0.0) {
		    outR = dstR;
		    outG = dstG;
		    outB = dstB;
		    outA = dstA;
		} else {
		    outA = srcA + dstA * (1.0 - srcA);
		    if (outA == 0.0) {
			outR = 0.0;
			outG = 0.0;
			outB = 0.0;
		    } else {
			outR = (srcR * srcA + dstR * dstA * (1.0 - srcA)) / outA;
			outG = (srcG * srcA + dstG * dstA * (1.0 - srcA)) / outA;
			outB = (srcB * srcA + dstB * dstA * (1.0 - srcA)) / outA;
		    }
		}

		out[x + 0] = (UINT8) (255.0 * outR);
		out[x + 1] = (UINT8) (255.0 * outG);
		out[x + 2] = (UINT8) (255.0 * outB);
		out[x + 3] = (UINT8) (255.0 * outA);

	    }

	}

    }

    return imOut;
}
