#ifndef GBAFE_STANDUNIT_H
#define GBAFE_STANDUNIT_H

#pragma long_calls

void HideUnitSMS(struct Unit* unit); //! FE8U = (0x0802810C+1)
void ShowUnitSMS(struct Unit* unit); //! FE8U = (0x08028130+1)

void SMS_UpdateFromGameData(void); //! FE8U = (0x80271A1)

#pragma long_calls_off

#endif // GBAFE_STANDUNIT_H
