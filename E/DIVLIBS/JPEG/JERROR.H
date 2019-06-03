/*
 * jerror.h
 *
 * Copyright (C) 1994-1995, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file defines the error and message codes for the JPEG library.
 * Edit this file to add new codes, or to translate the message strings to
 * some other language.
 * A set of error-reporting macros are defined too.  Some applications using
 * the JPEG library may wish to include this file to get the error codes
 * and/or the macros.
 */

/*
 * To define the enum list of message codes, include this file without
 * defining macro JMESSAGE.  To create a message string table, include it
 * again with a suitable JMESSAGE definition (see jerror.c for an example).
 */
#ifndef JMESSAGE
#ifndef JERROR_H
/* First time through, define the enum list */
#define JMAKE_ENUM_LIST
#else
/* Repeated inclusions of this file are no-ops unless JMESSAGE is defined */
#define JMESSAGE(code,string)
#endif /* JERROR_H */
#endif /* JMESSAGE */

#ifdef JMAKE_ENUM_LIST

typedef enum {

#define JMESSAGE(code,string)	code ,

#endif /* JMAKE_ENUM_LIST */

JMESSAGE(JMSG_NOMESSAGE, "JPEG_Bogus message code %d") /* Must be first entry! */

/* For maintenance convenience, list is alphabetical by message code name */
JMESSAGE(JERR_ARITH_NOTIMPL,
	 "JPEG_Sorry, there are legal restrictions on arithmetic coding")
JMESSAGE(JERR_BAD_ALIGN_TYPE, "JPEG_ALIGN_TYPE is wrong, please fix")
JMESSAGE(JERR_BAD_ALLOC_CHUNK, "JPEG_MAX_ALLOC_CHUNK is wrong, please fix")
JMESSAGE(JERR_BAD_BUFFER_MODE, "JPEG_Bogus buffer control mode")
JMESSAGE(JERR_BAD_COMPONENT_ID, "JPEG_Invalid component ID %d in SOS")
JMESSAGE(JERR_BAD_DCTSIZE, "JPEG_IDCT output block size %d not supported")
JMESSAGE(JERR_BAD_IN_COLORSPACE, "JPEG_Bogus input colorspace")
JMESSAGE(JERR_BAD_J_COLORSPACE, "JPEG_Bogus JPEG colorspace")
JMESSAGE(JERR_BAD_LENGTH, "JPEG_Bogus marker length")
JMESSAGE(JERR_BAD_LIB_VERSION,
	 "JPEG_Wrong JPEG library version: library is %d, caller expects %d")
JMESSAGE(JERR_BAD_MCU_SIZE, "JPEG_Sampling factors too large for interleaved scan")
JMESSAGE(JERR_BAD_POOL_ID, "JPEG_Invalid memory pool code %d")
JMESSAGE(JERR_BAD_PRECISION, "JPEG_Unsupported JPEG data precision %d")
JMESSAGE(JERR_BAD_PROGRESSION,
	 "JPEG_Invalid progressive parameters Ss=%d Se=%d Ah=%d Al=%d")
JMESSAGE(JERR_BAD_PROG_SCRIPT,
	 "JPEG_Invalid progressive parameters at scan script entry %d")
JMESSAGE(JERR_BAD_SAMPLING, "JPEG_Bogus sampling factors")
JMESSAGE(JERR_BAD_SCAN_SCRIPT, "JPEG_Invalid scan script at entry %d")
JMESSAGE(JERR_BAD_STATE, "JPEG_Improper call to JPEG library in state %d")
JMESSAGE(JERR_BAD_STRUCT_SIZE,
	 "JPEG_JPEG parameter struct mismatch: library thinks size is %u, caller expects %u")
JMESSAGE(JERR_BAD_VIRTUAL_ACCESS, "JPEG_Bogus virtual array access")
JMESSAGE(JERR_BUFFER_SIZE, "JPEG_Buffer passed to JPEG library is too small")
JMESSAGE(JERR_CANT_SUSPEND, "JPEG_Suspension not allowed here")
JMESSAGE(JERR_CCIR601_NOTIMPL, "JPEG_CCIR601 sampling not implemented yet")
JMESSAGE(JERR_COMPONENT_COUNT, "JPEG_Too many color components: %d, max %d")
JMESSAGE(JERR_CONVERSION_NOTIMPL, "JPEG_Unsupported color conversion request")
JMESSAGE(JERR_DAC_INDEX, "JPEG_Bogus DAC index %d")
JMESSAGE(JERR_DAC_VALUE, "JPEG_Bogus DAC value 0x%x")
JMESSAGE(JERR_DHT_COUNTS, "JPEG_Bogus DHT counts")
JMESSAGE(JERR_DHT_INDEX, "JPEG_Bogus DHT index %d")
JMESSAGE(JERR_DQT_INDEX, "JPEG_Bogus DQT index %d")
JMESSAGE(JERR_EMPTY_IMAGE, "JPEG_Empty JPEG image (DNL not supported)")
JMESSAGE(JERR_EMS_READ, "JPEG_Read from EMS failed")
JMESSAGE(JERR_EMS_WRITE, "JPEG_Write to EMS failed")
JMESSAGE(JERR_EOI_EXPECTED, "JPEG_Didn't expect more than one scan")
JMESSAGE(JERR_FILE_READ, "JPEG_Input file read error")
JMESSAGE(JERR_FILE_WRITE, "JPEG_Output file write error --- out of disk space?")
JMESSAGE(JERR_FRACT_SAMPLE_NOTIMPL, "JPEG_Fractional sampling not implemented yet")
JMESSAGE(JERR_HUFF_CLEN_OVERFLOW, "JPEG_Huffman code size table overflow")
JMESSAGE(JERR_HUFF_MISSING_CODE, "JPEG_Missing Huffman code table entry")
JMESSAGE(JERR_IMAGE_TOO_BIG, "JPEG_Maximum supported image dimension is %u pixels")
JMESSAGE(JERR_INPUT_EMPTY, "JPEG_Empty input file")
JMESSAGE(JERR_INPUT_EOF, "JPEG_Premature end of input file")
JMESSAGE(JERR_MISMATCHED_QUANT_TABLE,
	 "JPEG_Cannot transcode due to multiple use of quantization table %d")
JMESSAGE(JERR_MISSING_DATA, "JPEG_Scan script does not transmit all data")
JMESSAGE(JERR_MODE_CHANGE, "JPEG_Invalid color quantization mode change")
JMESSAGE(JERR_NOTIMPL, "JPEG_Not implemented yet")
JMESSAGE(JERR_NOT_COMPILED, "JPEG_Requested feature was omitted at compile time")
JMESSAGE(JERR_NO_BACKING_STORE, "JPEG_Backing store not supported")
JMESSAGE(JERR_NO_HUFF_TABLE, "JPEG_Huffman table 0x%02x was not defined")
JMESSAGE(JERR_NO_IMAGE, "JPEG_JPEG datastream contains no image")
JMESSAGE(JERR_NO_QUANT_TABLE, "JPEG_Quantization table 0x%02x was not defined")
JMESSAGE(JERR_NO_SOI, "JPEG_Not a JPEG file: starts with 0x%02x 0x%02x")
JMESSAGE(JERR_OUT_OF_MEMORY, "JPEG_Insufficient memory (case %d)")
JMESSAGE(JERR_QUANT_COMPONENTS,
	 "JPEG_Cannot quantize more than %d color components")
JMESSAGE(JERR_QUANT_FEW_COLORS, "JPEG_Cannot quantize to fewer than %d colors")
JMESSAGE(JERR_QUANT_MANY_COLORS, "JPEG_Cannot quantize to more than %d colors")
JMESSAGE(JERR_SOF_DUPLICATE, "JPEG_Invalid JPEG file structure: two SOF markers")
JMESSAGE(JERR_SOF_NO_SOS, "JPEG_Invalid JPEG file structure: missing SOS marker")
JMESSAGE(JERR_SOF_UNSUPPORTED, "JPEG_Unsupported JPEG process: SOF type 0x%02x")
JMESSAGE(JERR_SOI_DUPLICATE, "JPEG_Invalid JPEG file structure: two SOI markers")
JMESSAGE(JERR_SOS_NO_SOF, "JPEG_Invalid JPEG file structure: SOS before SOF")
JMESSAGE(JERR_TFILE_CREATE, "JPEG_Failed to create temporary file %s")
JMESSAGE(JERR_TFILE_READ, "JPEG_Read failed on temporary file")
JMESSAGE(JERR_TFILE_SEEK, "JPEG_Seek failed on temporary file")
JMESSAGE(JERR_TFILE_WRITE,
	 "JPEG_Write failed on temporary file --- out of disk space?")
JMESSAGE(JERR_TOO_LITTLE_DATA, "JPEG_Application transferred too few scanlines")
JMESSAGE(JERR_UNKNOWN_MARKER, "JPEG_Unsupported marker type 0x%02x")
JMESSAGE(JERR_VIRTUAL_BUG, "JPEG_Virtual array controller messed up")
JMESSAGE(JERR_WIDTH_OVERFLOW, "JPEG_Image too wide for this implementation")
JMESSAGE(JERR_XMS_READ, "JPEG_Read from XMS failed")
JMESSAGE(JERR_XMS_WRITE, "JPEG_Write to XMS failed")
JMESSAGE(JMSG_COPYRIGHT, JCOPYRIGHT)
JMESSAGE(JMSG_VERSION, JVERSION)
JMESSAGE(JTRC_16BIT_TABLES,
	 "JPEG_Caution: quantization tables are too coarse for baseline JPEG")
JMESSAGE(JTRC_ADOBE,
	 "JPEG_Adobe APP14 marker: version %d, flags 0x%04x 0x%04x, transform %d")
JMESSAGE(JTRC_APP0, "JPEG_Unknown APP0 marker (not JFIF), length %u")
JMESSAGE(JTRC_APP14, "JPEG_Unknown APP14 marker (not Adobe), length %u")
JMESSAGE(JTRC_DAC, "JPEG_Define Arithmetic Table 0x%02x: 0x%02x")
JMESSAGE(JTRC_DHT, "JPEG_Define Huffman Table 0x%02x")
JMESSAGE(JTRC_DQT, "JPEG_Define Quantization Table %d  precision %d")
JMESSAGE(JTRC_DRI, "JPEG_Define Restart Interval %u")
JMESSAGE(JTRC_EMS_CLOSE, "JPEG_Freed EMS handle %u")
JMESSAGE(JTRC_EMS_OPEN, "JPEG_Obtained EMS handle %u")
JMESSAGE(JTRC_EOI, "JPEG_End Of Image")
JMESSAGE(JTRC_HUFFBITS, "JPEG_        %3d %3d %3d %3d %3d %3d %3d %3d")
JMESSAGE(JTRC_JFIF, "JPEG_JFIF APP0 marker, density %dx%d  %d")
JMESSAGE(JTRC_JFIF_BADTHUMBNAILSIZE,
	 "JPEG_Warning: thumbnail image size does not match data length %u")
JMESSAGE(JTRC_JFIF_MINOR, "JPEG_Unknown JFIF minor revision number %d.%02d")
JMESSAGE(JTRC_JFIF_THUMBNAIL, "JPEG_    with %d x %d thumbnail image")
JMESSAGE(JTRC_MISC_MARKER, "JPEG_Skipping marker 0x%02x, length %u")
JMESSAGE(JTRC_PARMLESS_MARKER, "JPEG_Unexpected marker 0x%02x")
JMESSAGE(JTRC_QUANTVALS, "JPEG_        %4u %4u %4u %4u %4u %4u %4u %4u")
JMESSAGE(JTRC_QUANT_3_NCOLORS, "JPEG_Quantizing to %d = %d*%d*%d colors")
JMESSAGE(JTRC_QUANT_NCOLORS, "JPEG_Quantizing to %d colors")
JMESSAGE(JTRC_QUANT_SELECTED, "JPEG_Selected %d colors for quantization")
JMESSAGE(JTRC_RECOVERY_ACTION, "JPEG_At marker 0x%02x, recovery action %d")
JMESSAGE(JTRC_RST, "JPEG_RST%d")
JMESSAGE(JTRC_SMOOTH_NOTIMPL,
	 "JPEG_Smoothing not supported with nonstandard sampling ratios")
JMESSAGE(JTRC_SOF, "JPEG_Start Of Frame 0x%02x: width=%u, height=%u, components=%d")
JMESSAGE(JTRC_SOF_COMPONENT, "JPEG_    Component %d: %dhx%dv q=%d")
JMESSAGE(JTRC_SOI, "JPEG_Start of Image")
JMESSAGE(JTRC_SOS, "JPEG_Start Of Scan: %d components")
JMESSAGE(JTRC_SOS_COMPONENT, "JPEG_    Component %d: dc=%d ac=%d")
JMESSAGE(JTRC_SOS_PARAMS, "JPEG_  Ss=%d, Se=%d, Ah=%d, Al=%d")
JMESSAGE(JTRC_TFILE_CLOSE, "JPEG_Closed temporary file %s")
JMESSAGE(JTRC_TFILE_OPEN, "JPEG_Opened temporary file %s")
JMESSAGE(JTRC_UNKNOWN_IDS,
	 "JPEG_Unrecognized component IDs %d %d %d, assuming YCbCr")
JMESSAGE(JTRC_XMS_CLOSE, "JPEG_Freed XMS handle %u")
JMESSAGE(JTRC_XMS_OPEN, "JPEG_Obtained XMS handle %u")
JMESSAGE(JWRN_ADOBE_XFORM, "JPEG_Unknown Adobe color transform code %d")
JMESSAGE(JWRN_BOGUS_PROGRESSION,
	 "JPEG_Inconsistent progression sequence for component %d coefficient %d")
JMESSAGE(JWRN_EXTRANEOUS_DATA,
	 "JPEG_Corrupt JPEG data: %u extraneous bytes before marker 0x%02x")
JMESSAGE(JWRN_HIT_MARKER, "JPEG_Corrupt JPEG data: premature end of data segment")
JMESSAGE(JWRN_HUFF_BAD_CODE, "JPEG_Corrupt JPEG data: bad Huffman code")
JMESSAGE(JWRN_JFIF_MAJOR, "JPEG_Warning: unknown JFIF revision number %d.%02d")
JMESSAGE(JWRN_JPEG_EOF, "JPEG_Premature end of JPEG file")
JMESSAGE(JWRN_MUST_RESYNC,
	 "JPEG_Corrupt JPEG data: found marker 0x%02x instead of RST%d")
JMESSAGE(JWRN_NOT_SEQUENTIAL, "JPEG_Invalid SOS parameters for sequential JPEG")
JMESSAGE(JWRN_TOO_MUCH_DATA, "JPEG_Application transferred too many scanlines")

#ifdef JMAKE_ENUM_LIST

  JMSG_LASTMSGCODE
} J_MESSAGE_CODE;

#undef JMAKE_ENUM_LIST
#endif /* JMAKE_ENUM_LIST */

/* Zap JMESSAGE macro so that future re-inclusions do nothing by default */
#undef JMESSAGE


#ifndef JERROR_H
#define JERROR_H

/* Macros to simplify using the error and trace message stuff */
/* The first parameter is either type of cinfo pointer */

/* Fatal errors (print message and exit) */
#define ERREXIT(cinfo,code)  \
  ((cinfo)->err->msg_code = (code), \
   (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXIT1(cinfo,code,p1)  \
  ((cinfo)->err->msg_code = (code), \
   (cinfo)->err->msg_parm.i[0] = (p1), \
   (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXIT2(cinfo,code,p1,p2)  \
  ((cinfo)->err->msg_code = (code), \
   (cinfo)->err->msg_parm.i[0] = (p1), \
   (cinfo)->err->msg_parm.i[1] = (p2), \
   (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXIT3(cinfo,code,p1,p2,p3)  \
  ((cinfo)->err->msg_code = (code), \
   (cinfo)->err->msg_parm.i[0] = (p1), \
   (cinfo)->err->msg_parm.i[1] = (p2), \
   (cinfo)->err->msg_parm.i[2] = (p3), \
   (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXIT4(cinfo,code,p1,p2,p3,p4)  \
  ((cinfo)->err->msg_code = (code), \
   (cinfo)->err->msg_parm.i[0] = (p1), \
   (cinfo)->err->msg_parm.i[1] = (p2), \
   (cinfo)->err->msg_parm.i[2] = (p3), \
   (cinfo)->err->msg_parm.i[3] = (p4), \
   (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXITS(cinfo,code,str)  \
  ((cinfo)->err->msg_code = (code), \
   strncpy((cinfo)->err->msg_parm.s, (str), JMSG_STR_PARM_MAX), \
   (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))

#define MAKESTMT(stuff)		do { stuff } while (0)

/* Nonfatal errors (we can keep going, but the data is probably corrupt) */
#define WARNMS(cinfo,code)  \
  ((cinfo)->err->msg_code = (code), \
   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), -1))
#define WARNMS1(cinfo,code,p1)  \
  ((cinfo)->err->msg_code = (code), \
   (cinfo)->err->msg_parm.i[0] = (p1), \
   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), -1))
#define WARNMS2(cinfo,code,p1,p2)  \
  ((cinfo)->err->msg_code = (code), \
   (cinfo)->err->msg_parm.i[0] = (p1), \
   (cinfo)->err->msg_parm.i[1] = (p2), \
   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), -1))

/* Informational/debugging messages */
#define TRACEMS(cinfo,lvl,code)  \
  ((cinfo)->err->msg_code = (code), \
   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)))
#define TRACEMS1(cinfo,lvl,code,p1)  \
  ((cinfo)->err->msg_code = (code), \
   (cinfo)->err->msg_parm.i[0] = (p1), \
   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)))
#define TRACEMS2(cinfo,lvl,code,p1,p2)  \
  ((cinfo)->err->msg_code = (code), \
   (cinfo)->err->msg_parm.i[0] = (p1), \
   (cinfo)->err->msg_parm.i[1] = (p2), \
   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)))
#define TRACEMS3(cinfo,lvl,code,p1,p2,p3)  \
  MAKESTMT(int * _mp = (cinfo)->err->msg_parm.i; \
	   _mp[0] = (p1); _mp[1] = (p2); _mp[2] = (p3); \
	   (cinfo)->err->msg_code = (code); \
	   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)); )
#define TRACEMS4(cinfo,lvl,code,p1,p2,p3,p4)  \
  MAKESTMT(int * _mp = (cinfo)->err->msg_parm.i; \
	   _mp[0] = (p1); _mp[1] = (p2); _mp[2] = (p3); _mp[3] = (p4); \
	   (cinfo)->err->msg_code = (code); \
	   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)); )
#define TRACEMS8(cinfo,lvl,code,p1,p2,p3,p4,p5,p6,p7,p8)  \
  MAKESTMT(int * _mp = (cinfo)->err->msg_parm.i; \
	   _mp[0] = (p1); _mp[1] = (p2); _mp[2] = (p3); _mp[3] = (p4); \
	   _mp[4] = (p5); _mp[5] = (p6); _mp[6] = (p7); _mp[7] = (p8); \
	   (cinfo)->err->msg_code = (code); \
	   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)); )
#define TRACEMSS(cinfo,lvl,code,str)  \
  ((cinfo)->err->msg_code = (code), \
   strncpy((cinfo)->err->msg_parm.s, (str), JMSG_STR_PARM_MAX), \
   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)))

#endif /* JERROR_H */
