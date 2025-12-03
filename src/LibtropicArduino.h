#ifndef LIBTROPIC_ARDUINO_H
#define LIBTROPIC_ARDUINO_H

/**
 * @file LibtropicArduino.h
 * @brief Declarations of the Libtropic C++ wrapper.
 * @copyright Copyright (c) 2020-2025 Tropic Square s.r.o.
 *
 * @license For the license see file LICENSE.txt file in the root directory of this source tree.
 */

#include <Arduino.h>
#include <SPI.h>

#include "libtropic.h"
#include "libtropic_common.h"
#include "libtropic_mbedtls_v4.h"
#include "libtropic_port_arduino.h"

/**
 * @brief Instance of this class is used to communicate with one TROPIC01 chip.
 *
 */
class Tropic01 {
   public:
    /**
     * @brief Tropic01 destructor. Calls Tropic01.end().
     *
     */
    ~Tropic01(void);

    /**
     * @brief Initializes resources. Must be called before all other methods are called.
     * @note Number of arguments depends on some Libtropic's CMake options.
     *
     * @retval  LT_OK Method executed successfully
     * @retval  other Method did not execute successully, you might use lt_ret_verbose() to get verbose
     * encoding of returned value
     */
    lt_ret_t begin(const uint16_t spi_cs_pin
#if LT_USE_INT_PIN
                   ,
                   const uint16_t int_gpio_pin
#endif
#if LT_SEPARATE_L3_BUFF
                   ,
                   uint8_t *l3_buff, const uint16_t l3_buff_len
#endif
                   ,
                   const unsigned int rng_seed = random(), SPIClass &spi = ::SPI,
                   SPISettings spi_settings = SPISettings(10000000, MSBFIRST, SPI_MODE0));

    /**
     * @brief Deinitialize resources. Should be called at the end of the program.
     *
     * @retval  LT_OK Method executed successfully
     * @retval  other Method did not execute successully, you might use lt_ret_verbose() to get verbose
     * encoding of returned value
     */
    lt_ret_t end(void);

    /**
     * @brief Establishes Secure Session Channel with TROPIC01.
     *
     * @retval  LT_OK Method executed successfully
     * @retval  other Method did not execute successully, you might use lt_ret_verbose() to get verbose
     * encoding of returned value
     */
    lt_ret_t secureSessionStart(const uint8_t *shipriv, const uint8_t *shipub, const lt_pkey_index_t pkey_index);

    /**
     * @brief Aborts Secure Channel Session with TROPIC01.
     *
     * @retval  LT_OK Method executed successfully
     * @retval  other Method did not execute successully, you might use lt_ret_verbose() to get verbose
     * encoding of returned value
     */
    lt_ret_t secureSessionEnd(void);

    /**
     * @brief Executes the TROPIC01's Ping command. It is a dummy command to check the Secure Channel Session
     * is valid by exchanging a message with TROPIC01, which is echoed through the Secure Channel.
     *
     * @retval  LT_OK Method executed successfully
     * @retval  other Method did not execute successully, you might use lt_ret_verbose() to get verbose
     * encoding of returned value
     */
    lt_ret_t ping(const uint8_t *msg_out, uint8_t *msg_in, const uint16_t msg_len);

   private:
    lt_dev_arduino_t device;
    lt_ctx_mbedtls_v4_t crypto_ctx;
    lt_handle_t handle;
};

#endif  // LIBTROPIC_ARDUINO_H