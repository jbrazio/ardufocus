/**
 * Circular Queue - Implementation of the classic ring buffer data structure
 * Copyright (C) 2017 João Brázio [joao@brazio.org]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "macro.h"

/**
 * @brief   Circular Queue class
 * @details Implementation of the classic ring buffer data structure
 */
template<typename T, uint8_t N> class Ringbuf
{
private:
  /**
   * @brief   Buffer structure
   * @details This structure consolidates all the overhead required to handle
   *          a circular queue such as the pointers and the buffer vector.
   */
  struct {
    T queue[N];
    uint8_t head;
    volatile uint8_t tail;
  } m_buffer;

public:
  /**
   * @brief   Class constructor
   * @details This class requires two template parameters, T defines the type
   *          of item this queue will handle and N defines the maximum number of
   *          items that can be stored on the queue.
   */
  Ringbuf() { reset(); }

  /**
   * @brief   Adds an item to the queue
   * @details Adds an item to the queue on the location pointed by the buffer_t
   *          tail variable. Returns false if no queue space is available.
   * @param   item Item to be added to the queue
   * @return  true if the operation was successful
   */
  bool enqueue(T const &item)
  {
    if (full()) return false;

    CRITICAL_SECTION_START
      m_buffer.queue[m_buffer.tail] = item;
      m_buffer.tail = (m_buffer.tail +1) % N;
    CRITICAL_SECTION_END

    return true;
  }

  /**
   * @brief   Checks if the queue has no items
   * @details Returns true if there are no items on the queue, false otherwise.
   * @return  true if queue is empty
   */
  inline bool empty() { return (abs(m_buffer.head == m_buffer.tail)); }

  /**
   * @brief   Checks if the queue is full
   * @details Returns true if the queue is full, false otherwise.
   * @return  true if queue is full
   */
  inline bool full() { return (m_buffer.head == (m_buffer.tail +1) % N); }

  /**
   * @brief   Gets the next element from the queue without removing it
   * @details Returns the next element in the queue without removing it
   *          or updating the pointers.
   * @return  first elements in the queue
   */
  inline T peek() { return m_buffer.queue[m_buffer.head]; }

  /**
   * @brief   Reset the queue position
   * @details The head and tail pointer will be reset to the start position thus
   *          the queue will become empty. Please be aware that the data will not
   *          be removed from memory.
   * @return  true if the operation was successful
   */
  bool reset()
  {
    CRITICAL_SECTION_START
      m_buffer.head = 0;
      m_buffer.tail = 0;
    CRITICAL_SECTION_END
    return true;
  }

  /**
   * @brief   Gets the number of elements on the queue
   * @details Returns the number of elements in the underlying container,
   *          that is, the number of elements in queue[].
   * @return  the number of elements in the container
   */
  inline uint8_t size() { return (m_buffer.tail - m_buffer.head); }

  /**
   * @brief   Removes and returns a element from the queue
   * @details Removes the oldest element on the queue, pointed to by the
   *          buffer_t head field. The element is returned to the caller.
   * @return  type T element
   */
  T dequeue()
  {
    if (empty()) return T();

    CRITICAL_SECTION_START
      const T item = m_buffer.queue[m_buffer.head];
      m_buffer.head = (m_buffer.head +1) % N;
    CRITICAL_SECTION_END

    return item;
  }
};

#endif
