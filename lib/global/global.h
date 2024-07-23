#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>

extern boolean device_has_been_selected;
extern boolean mode_has_been_selected;

static const uint8_t MAX_DEVICES = 3;
static const uint8_t MAX_BUTTONS = 6;
static const uint8_t MAX_MODES = 10; // Maximum possible modes for any device

// Device IDs
typedef enum DeviceID
{
    PRIM,
    PROPILOT,
    DUO
} D_ID;

// Device Names
const String DEVICE_NAMES[MAX_DEVICES] = {"PRIM", "PROPILOT", "DUO"};

// Button Names for Each Device
const String BUTTON_NAMES[][MAX_BUTTONS] = {
    {"ON/OFF", "SET", "CAL"},                                    // PRIM
    {"MENU/ESC", "SET", "UP", "DOWN", "CAL/OK"},                 // PROPILOT
    {"ON/OFF_1", "SET_1", "CAL_1", "ON/OFF_2", "SET_2", "CAL_2"} // DUO
};

// Mode Names for Each Device
const String MODE_NAMES[][MAX_MODES] = {
    {"AP1", "BP1", "AR1", "AR3"}, // PRIM (4 modes)
    {},                           // PROPILOT (x modes)
    {}                            // DUO (x modes)
};

// Number of Buttons and Modes for Each Device
const uint8_t NUM_BUTTONS[] = {3, 5, 6}; // Number of buttons for {PRIM, PROPILOT, DUO}
const uint8_t NUM_MODES[] = {4, 5, 10};  // Number of modes for {PRIM, PROPILOT, DUO}

// Device Structure
typedef struct DEVICE
{

    String Device_name;
    uint8_t Device_ID;
    String button_names[MAX_BUTTONS];
    String mode_names[MAX_MODES];
    uint8_t number_of_buttons;
    uint8_t number_of_modes;

    // DEVICE(uint8_t ID) : Device_ID(ID), Device_name(DEVICE_NAMES[ID])
    // {
    //     // Copy button names
    //     for (uint8_t i = 0; i < NUM_BUTTONS[ID]; ++i)
    //     {
    //         button_names[i] = BUTTON_NAMES[ID][i];
    //     }

    //     // Copy mode names
    //     for (uint8_t i = 0; i < NUM_MODES[ID]; ++i)
    //     {
    //         mode_names[i] = MODE_NAMES[ID][i];
    //     }

    //     // Set number of buttons and modes
    //     number_of_buttons = NUM_BUTTONS[ID];
    //     number_of_modes = NUM_MODES[ID];
    // }

    /*!
     * @brief SET_DEVICE
     * @param ID ID of device chosen {PRIM = 0, PROPILOT = 1, DUO = 2}
     */
    void SET_DEVICE(uint8_t ID)
    {
        Device_ID = ID;
        Device_name = DEVICE_NAMES[ID];
        for (uint8_t i = 0; i < NUM_BUTTONS[ID]; ++i)
        {
            button_names[i] = BUTTON_NAMES[ID][i];
        }

        // Copy mode names
        for (uint8_t i = 0; i < NUM_MODES[ID]; ++i)
        {
            mode_names[i] = MODE_NAMES[ID][i];
        }

        // Set number of buttons and modes
        number_of_buttons = NUM_BUTTONS[ID];
        number_of_modes = NUM_MODES[ID];
    }
} DEVICE;

extern DEVICE myDEVICE;

// Mode Structure
typedef struct MODE
{
    String Mode_name;
    uint8_t Mode_ID;

    // MODE(uint8_t ID) : Mode_ID(ID), Mode_name(DEVICE_NAMES[ID])
    // {
    // }
} MODE;
extern MODE myMODE;

// Example Function
void SET_DATA();

#endif // GLOBAL_H
