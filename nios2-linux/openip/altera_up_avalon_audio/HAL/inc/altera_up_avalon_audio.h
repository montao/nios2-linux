#ifndef __ALTERA_UP_AVALON_AUDIO_H__
#define __ALTERA_UP_AVALON_AUDIO_H__

#include <stddef.h>
#include <alt_types.h>
#include <sys/alt_dev.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define LEFT 0
#define RIGHT 1

/*
 * Device structure definition. Each instance of the driver uses one
 * of these structures to hold its associated state.
 */
typedef struct alt_up_audio_dev {
	/// @brief character mode device structure 
	/// @sa Developing Device Drivers for the HAL in Nios II Software Developer's Handbook
	alt_dev dev;
	/// @brief the base address of the device
	unsigned int base;
	/// @brief choose the right or left channel
	unsigned channel;
} alt_up_audio_dev;


//////////////////////////////////////////////////////////////////////////
// HAL system functions

//////////////////////////////////////////////////////////////////////////
// file-like operation functions

//////////////////////////////////////////////////////////////////////////
// direct operation functions

/**
 * @brief Open the Audio device specified by <em> name </em>
 *
 * @param name -- the Audio component name in SOPC Builder. 
 *
 * @return The corresponding device structure, or NULL if the device is not found
 *
 * @par Thread-safe
 * 	No
 * @par Commonly called by
 *  User program
 * @par Available from ISR
 *  ---
 **/
alt_up_audio_dev* alt_up_audio_open_dev(const char* name);

/**
 * @brief Enable the read interrupts for the Audio Core
 *
 * @param audio -- the audio device structure 
 *
 * @return 0 for success
 *
 * @par Thread-safe
 * 	No
 * @par Commonly called by
 *  User program
 * @par Available from ISR
 *  ---
 **/
int alt_up_audio_enable_read_interrupt(alt_up_audio_dev *audio);

/**
 * @brief Disable the read interrupts for the Audio Core
 *
 * @param audio -- the audio device structure 
 *
 * @return 0 for success
 *
 * @par Thread-safe
 * 	No
 * @par Commonly called by
 *  User program
 * @par Available from ISR
 *  ---
 **/
int alt_up_audio_disable_read_interrupt(alt_up_audio_dev *audio);

/**
 * @brief Reset the Audio Core by clearing the input and output FIFOs for both the left channel and the right channel
 *
 * @param audio -- the audio device structure 
 *
 * @return 0 for success
 *
 * @par Thread-safe
 * 	No
 * @par Commonly called by
 *  User program
 * @par Available from ISR
 *  ---
 **/
int alt_up_audio_reset_audio_core(alt_up_audio_dev *audio);

/**
 * @brief Read \em len words of data from left input FIFO and right input FIFO,
 * respectively, and store data to where \em buf points
 *
 * @param audio -- the audio device structure 
 * @param buf  -- the pointer to the allocated memory for storing audio data.
 * Size of \em buf should be no smaller than <em>2*len</em> words. The first
 * <em>len</em> words will store data from the left channel while the second
 * stores data from the right channel.

 * @param len  -- the number of data in words to read from each input FIFO
 *
 * @return the total number of words read
 *
 * @note The function will read the FIFO until <em>2*len</em> is reached or the FIFO is empty.
 *
 * @par Thread-safe
 * 	No
 * @par Commonly called by
 *  User program
 * @par Available from ISR
 *  ---
 **/
int alt_up_audio_read_fifo(alt_up_audio_dev *audio, alt_u32 *buf, unsigned len);

/**
 * @brief Write \em len words of data from \em buf to each of the output FIFO (left and right)
 *
 * @param audio -- the audio device structure 
 * @param buf  -- the pointer to the data to be written
 * Size of \em buf should be no smaller than <em>2*len</em> words. The first
 * <em>len</em> words will store data for the left channel while the second
 * stores data for the right channel
 * @param len  -- the number of data in words to be written into each output FIFO
 *
 * @return the total number of data written
 *
 * @note The function will write to the FIFO until \em 2*len is reached or the FIFO is full.
 *
 * @par Thread-safe
 * 	No
 * @par Commonly called by
 *  User program
 * @par Available from ISR
 *  ---
 **/
int alt_up_audio_write_fifo(alt_up_audio_dev *audio, alt_u32 *buf, unsigned len);

/*
 * Macros used by alt_sys_init 
 */
#define ALTERA_UP_AVALON_AUDIO_INSTANCE(name, device)	\
  static alt_up_audio_dev device =		\
  {                                                 	\
    {                                               	\
      ALT_LLIST_ENTRY,                              	\
      name##_NAME,                                  	\
      NULL , /* open */		\
      NULL , /* close */		\
      NULL, /* read */		\
      NULL, /* write */		\
      NULL , /* lseek */		\
      NULL , /* fstat */		\
      NULL , /* ioctl */		\
    },                                              	\
    {                                               	\
        name##_BASE,                                	\
    },													\
	0	/* 0 for LEFT */								\
  }

#define ALTERA_UP_AVALON_AUDIO_INIT(name, device) \
{	\
    alt_dev_reg(&device.dev);                          	\
}



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ALTERA_UP_AVALON_AUDIO_H__ */


