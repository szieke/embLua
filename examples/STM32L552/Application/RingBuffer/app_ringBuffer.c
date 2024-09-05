#include <app_ringBuffer.h>

/*************************************************************************/

void ringBuf_construct(ringBuf_Handle object, unsigned char *bufPtr, size_t bufSize)
{
  object->buffer = bufPtr;
  object->length = bufSize;
  object->count = 0;
  object->head = bufSize - 1;
  object->tail = 0;
  object->maxCount = 0;
}

/*************************************************************************/

size_t ringBuf_getCount(ringBuf_Handle object)
{
  return object->count;
}

/*************************************************************************/

size_t ringBuf_getMaxCount(ringBuf_Handle object)
{
  return object->maxCount;
}

/*************************************************************************/

size_t ringBuf_getData(ringBuf_Handle object, unsigned char *data, size_t n, ringBuf_disableCommunicationIsr disableIsr,
    ringBuf_enableCommunicationIsr enableIsr)
{
  size_t removed = 0;

  if (disableIsr)
  {
    disableIsr();
  }

  if (n > object->count)
  {
    n = object->count;
  }
  while (n)
  {
    if (disableIsr)
    {
      disableIsr();
    }
    *data++ = object->buffer[object->tail++];
    object->tail %= object->length;
    object->count--;

    if (enableIsr)
    {
      enableIsr();
    }

    n--;
    removed++;
  }

  if (enableIsr)
  {
    enableIsr();
  }

  return removed;
}

/*************************************************************************/

void ringBuf_clear(ringBuf_Handle object)
{
  size_t maxCount;

  maxCount = object->maxCount;
  ringBuf_construct(object, object->buffer, object->length);
  object->maxCount = maxCount;
}

/*************************************************************************/

bool ringBuf_isFull(ringBuf_Handle object)
{
  return (object->count == object->length);
}

/*************************************************************************/

size_t ringBuf_putData(ringBuf_Handle object, unsigned char *data, size_t n, ringBuf_disableCommunicationIsr disableIsr,
    ringBuf_enableCommunicationIsr enableIsr)
{
  size_t next;
  size_t added = 0;

  if (disableIsr)
  {
    disableIsr();
  }

  if (n > ringBuf_space(object))
  {
    n = ringBuf_space(object);
  }
  while (n != 0)
  {
    if (disableIsr)
    {
      disableIsr();
    }

    next = (object->head + 1) % object->length;
    object->buffer[next] = *data++;
    object->head = next;
    object->count++;

    if (enableIsr)
    {
      enableIsr();
    }

    n--;
    added++;
  }
  if (object->maxCount < object->count)
  {
    object->maxCount = object->count;
  }

  if (enableIsr)
  {
    enableIsr();
  }

  return added;
}
