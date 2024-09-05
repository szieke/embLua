#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The ring buffer structure.
 */
typedef struct
{
  unsigned char *buffer;
  size_t length;
  size_t count;
  size_t head;
  size_t tail;
  size_t maxCount;
} ringBuf_Object, *ringBuf_Handle;

/**
 *  Initialize the circular buffer.
 *
 *  @param object Pointer to the ring buffer object.
 *  @param bufPtr Pointer to data buffer to be used for the circular buffer.
 *                The buffer is NOT stored in ringBuf_Object.
 *  @param bufSize The size of bufPtr in number of unsigned chars.
 */
void ringBuf_construct(ringBuf_Handle object, unsigned char *bufPtr, size_t bufSize);

/**
 *  Clears all the data from the buffer.
 *
 *  @param object Pointer to the ring buffer object.
 */
void ringBuf_clear(ringBuf_Handle object);

/**
 *  Get the number of unsigned chars currently stored on the circular
 *          buffer.
 *
 *  @param object Pointer to the ring buffer object.
 *  @return Number of unsigned chars on the circular buffer.
 */
size_t ringBuf_getCount(ringBuf_Handle object);

/**
 *  A high-water mark indicating the largest number of unsigned chars
 *  stored on the circular buffer since it was constructed.
 *
 *  @param object Pointer to the ring buffer object.
 *  @return Get the largest number of unsigned chars that were at one
 *          point in the circular buffer.
 */
size_t ringBuf_getMaxCount(ringBuf_Handle object);

/**
 * Pointer to a function that disables an ISR during the access on the buffer.
 */
typedef void (*ringBuf_disableCommunicationIsr)(void);

/**
 * Pointer to a function that enables the ISR that was disabled in a ring buffer function.
 */
typedef void (*ringBuf_enableCommunicationIsr)(void);

/**
 *  Get one or more unsigned chars from the end of the circular buffer and
 *  remove them.
 *
 *  @param object Pointer to the ring buffer object.
 *  @param data Pointer to an unsigned char to be filled with the data from
 *              the front of the circular buffer.
 *  @param n The number of unsigned chars to try and remove.
 *  @param disableIsr Pointer to a function that disables an ISR during the access on the buffer. Can be NULL.
 *  @param denableIsr Pointer to a function that enables the ISR that is disabled with disableIsr. Can be NULL.
 *  @return Number of unsigned chars successfully removed from the buffer
 *          and copied into data. May be 0 or less than n.
 */
size_t ringBuf_getData(ringBuf_Handle object, unsigned char *data, size_t n, ringBuf_disableCommunicationIsr disableIsr,
    ringBuf_enableCommunicationIsr enableIs);

/**
 *  Function to determine if the circular buffer is full or not.
 *
 *  @param object Pointer to the ring buffer object.
 *  @return True if circular buffer is full, else FALSE.
 */
bool ringBuf_isFull(ringBuf_Handle object);

/*
 *  Put one or more unsigned chars into the end of the circular buffer.
 *
 *  @param object The ring buffer object.
 *  @param data   The unsigned chars to be placed at the end of the circular
 *                buffer.
 *  @param n The number of unsigned chars to try and remove.
 *  @param disableIsr Pointer to a function that disables an ISR during the access on the buffer. Can be NULL.
 *  @param denableIsr Pointer to a function that enables the ISR that is disabled with disableIsr. Can be NULL.
 *  @return Number of unsigned chars placed into the buffer. May be 0 or
 *          less than n.
 */
size_t ringBuf_putData(ringBuf_Handle object, unsigned char *data, size_t nr, ringBuf_disableCommunicationIsr disableIsr,
    ringBuf_enableCommunicationIsr enableIs);

/**
 *  Return the number of unsigned characters that the buffer has space for.
 *  This function should normally be called from a context where HWI is
 *  disabled. For efficiency, it is incumbent on the caller to ensure mutual
 *  exclusion with the proper HWI lock.
 *
 *  @param  object  Pointer to the ring buffer object.
 *  @return         Number of unsigned chars that the buffer has space for.
 */
static inline size_t ringBuf_space(ringBuf_Handle object)
{
  return (object->length - object->count);
}

#ifdef __cplusplus
}
#endif

#endif /* RING_BUFFER_H */
