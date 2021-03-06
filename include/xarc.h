/* File: xarc.h
 * The XARC user interface (C API).
 */

/* Copyright 2013 John Eubank.

   This file is part of XARC.

   XARC is free software: you can redistribute it and/or modify it under the
   terms of the GNU Lesser General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   XARC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
   more details.

   You should have received a copy of the GNU Lesser General Public License
   along with XARC.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef XARC_H_INC
#define XARC_H_INC

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>
#include <xarc/xchar.h>


/* Section: Types */


/* Type: xarc
 * An opaque pointer to the XARC implementation object.
 */
typedef void xarc;
/* Type: xarc_result_t
 * An integer code indicating the result of an XARC function.
 * See: <XARC result codes>
 */
typedef int8_t xarc_result_t;
/* Type: xarc_time_t
 * A platform-independent representation of a timestamp.
 *
 * Members:
 *   seconds - The number of seconds since the start of 1970, UTC
 *   nano - The number of nanoseconds since the start of the second
 */
typedef struct
{
	uintmax_t seconds;
	uint32_t nano;
} xarc_time_t;
/* Callback: xarc_extract_callback
 * A callback function from <xarc_item_extract>, called when a file or
 * directory is created.
 *
 * Parameters:
 *   param - The "param" parameter to <xarc_item_extract> is passed through to
 *     your callback
 *   path - The path, relative to <xarc_item_extract>'s "base_path", of the file
 *     or directory
 *   properties - The platform-independent properties of the file or
 *     directory (see <XARC entry properties>)
 */
typedef void (*xarc_extract_callback)(void* param, const xchar* path,
 uint8_t properties);
/* Struct: xarc_item_info
 * Properties of an entry within an archive.
 */
typedef struct
{
	/* Variable: path
	 * The entry's relative path.
	 */
	const xchar* path;
	/* Variable: properties
	 * Platform-independent properties of the entry.
	 * See: <XARC entry properties>
	 */
	uint8_t properties;
	/* Variable: mod_time
	 * The timestamp of the entry's last modification.
	 * See: <xarc_time_t>
	 */
	xarc_time_t mod_time;
} xarc_item_info;


/* Section: Global Functions */


/* Function: xarc_open
 * Open an archive for reading.
 *
 * Parameters:
 *   file - The path to the archive file to open
 *   type - Specify the type of the archive (see <XARC archive types>); use "0"
 *     to autodetect the archive type by the file extension
 *
 * Returns:
 *   A pointer to an <xarc> object that must always be freed with <xarc_close>,
 *   even if xarc_open was unsuccessful. You should always call <xarc_ok> on the
 *   object to see whether the archive was opened successfully.
 */
xarc* xarc_open(const xchar* file, uint8_t type);
/* Function: xarc_close
 * Closes and deallocates an <xarc> object received from <xarc_open>.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to deallocate
 *
 * Returns:
 *   An <xarc_result_t> indicating whether all deallocation functions were
 *   successful. (See <XARC result codes>.)
 */
xarc_result_t xarc_close(xarc* x);
/* Function: xarc_ok
 * Check if the <xarc> object is in a normal state and no errors have occurred.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to check
 *
 * Returns:
 *   Non-zero (evaluating to boolean TRUE) if no errors have occurred while
 *   working with the <xarc> object; 0 if an error has occurred.
 */
int8_t xarc_ok(xarc* x);

/* Function: xarc_error_id
 * Returns the error code generated by an XARC API function.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to retrieve data from
 *
 * Returns:
 *   <xarc_result_t> - The error code generated by XARC, or 0 if no error has
 *     occurred (see <XARC result codes>).
 */
xarc_result_t xarc_error_id(xarc* x);
/* Function: xarc_library_error_id
 * Returns the error code generated by an XARC module or decompression library.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to retrieve data from
 *
 * Returns:
 *   The error code generated by the module, or 0 if no error has occurred.
 */
int32_t xarc_library_error_id(xarc* x);
/* Function: xarc_error_description
 * Returns a descriptive string of the most recent error to occur.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to retrieve data from
 *
 * Returns:
 *   Pointer to a const string describing the error, valid at least until the
 *   next XARC function call; or the empty string ("") if no error has occurred.
 *
 * See also:
 *   <xarc_error>, <xarc_error_id>
 */
const xchar* xarc_error_description(xarc* x);
/* Function: xarc_error_additional
 * Returns any additional descriptive text generated at the time of the error.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to retrieve data from
 *
 * Returns:
 *   A const string with any additional text generated at the time of the
 *   error, such as actions being performed and the name of the file being
 *   processed; or the empty string ("") if no error has occurred or no
 *   additional text was generated.
 */
const xchar* xarc_error_additional(xarc* x);

/* Function: xarc_next_item
 * Move to the next entry in the archive.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to operate on
 *
 * Returns:
 *   XARC_OK - If there was a next entry in the archive to move to
 *   XARC_NO_MORE_ITEMS - If the <xarc> object was already at the last entry in
 *     the archive
 *   <xarc_result_t> - Any other error that may have occurred (see <XARC result
 *     codes>)
 */
xarc_result_t xarc_next_item(xarc* x);
/* Function: xarc_item_get_info
 * Retrieve the properties of an archive entry.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to retrieve data from
 *   info - Pointer to an <xarc_item_info> object to receive the properties
 *
 * Returns:
 *   XARC_OK - If no errors occurred
 *   <xarc_result_t> - Any error that may have occurred (see <XARC result
 *     codes>)
 */
xarc_result_t xarc_item_get_info(xarc* x, xarc_item_info* info);
/* Function: xarc_item_extract
 * Extract the current archive entry to a base path in the file system.
 *
 * Parameters:
 *   x - Pointer to the <xarc> object to extract from
 *   base_path - The base path in the local file system to extract to (if the
 *     entry contains subdirectories, these will be created below the base path)
 *   flags - Options controlling the extraction process (see <XARC extraction
 *     flags>)
 *   callback - A callback function that is called whenever a file or directory
 *     is created, or NULL for no callbacks (see <xarc_extract_callback>)
 *   callback_param - A parameter that is passed along unchanged to the callback
 *     function (use this to communicate state between the user calling code and
 *     the callback code)
 *
 * Returns:
 *   XARC_OK - If no errors occurred
 *   <xarc_result_t> - Any error that may have occurred (see <XARC result
 *     codes>)
 */
xarc_result_t xarc_item_extract(xarc* x, const xchar* base_path, uint8_t flags,
 xarc_extract_callback callback, void* callback_param);


/* Section: Identifiers */


/* Defines: XARC result codes
 * Numeric codes for possible success and failure modes in the xarc functions.
 *
 * (0) XARC_OK - No error
 * (1) XARC_NO_MORE_ITEMS - The <xarc> object attempted to move past the last
 *   entry in the archive
 * (2) XARC_DECOMPRESS_EOF - The end of the decompression stream in an XARC
 *   decompressor was reached (see <XARC decompressors>)
 * (-1) XARC_MODULE_ERROR - An error was encountered in the archive library
 *   being used by the current <xarc> object (see <XARC modules>)
 * (-2) XARC_DECOMPRESS_ERROR - An error was encountered in the XARC
 *   decompressor being used by the current <xarc> object (see <XARC
 *   decompressors>)
 * (-3) XARC_FILESYSTEM_ERROR - An error was encountered while working with the
 *   local file system or standard library functions
 * (-4) XARC_ERR_UNRECOGNIZED_ARCHIVE - The specified file didn't match any of
 *   the known archive types
 * (-5) XARC_ERR_UNRECOGNIZED_COMPRESSION - The decompressor failed to open the
 *   specified file as a decompression stream (see <XARC decompressors>)
 * (-6) XARC_ERR_NOT_VALID_ARCHIVE - The specified file was not a valid archive
 *   for the archive library that was chosen (see <XARC modules>)
 * (-7) XARC_ERR_DIR_IS_FILE - Failed to create a directory because a file
 *   already existed with the same name as the directory
 * (-8) XARC_ERR_NO_BASE_PATH - Tried to extract an archive entry to a base path
 *   that didn't exist
 * (-9) XARC_ERR_MEMORY - Failed while allocating or freeing memory
 */
#define XARC_OK								0
#define XARC_NO_MORE_ITEMS					1
#define XARC_DECOMPRESS_EOF					2
#define XARC_MODULE_ERROR					-1
#define XARC_DECOMPRESS_ERROR				-2
#define XARC_FILESYSTEM_ERROR				-3
#define XARC_ERR_UNRECOGNIZED_ARCHIVE		-4
#define XARC_ERR_UNRECOGNIZED_COMPRESSION	-5
#define XARC_ERR_NOT_VALID_ARCHIVE			-6
#define XARC_ERR_DIR_IS_FILE				-7
#define XARC_ERR_NO_BASE_PATH				-8
#define XARC_ERR_MEMORY						-9

/* Defines: XARC extraction flags
 * Options governing the extraction process.
 *
 * (0x1) XARC_XFLAGS_CALLBACK_DIRS - Set this flag to receive a callback when
 *   each directory item in an archive entry's relative path is created, in
 *   addition to the file itself. If the directory specified in an entry's
 *   relative path already exists, there will _not_ be a callback.
 */
#define XARC_XFLAG_CALLBACK_DIRS	0x1

/* Defines: XARC entry properties
 * Properties of an archive entry.
 *
 * (0x1) XARC_PROP_DIR - The entry is a directory, not a file
 */
#define XARC_PROP_DIR	0x1


/* Create named constants for each known archive type */
#define XARC_TYPE_BEGIN(id, name) \
 extern const uint8_t name;
#define XARC_EXTENSION(ext)
#define XARC_TYPE_END()
#include <xarc/types.inc>
#undef XARC_TYPE_BEGIN
#undef XARC_EXTENSION
#undef XARC_TYPE_END


#ifdef __cplusplus
} // extern "C"
#endif

#endif // XARC_H_INC
