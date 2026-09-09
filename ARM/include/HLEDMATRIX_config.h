#ifndef HLEDMATRIX_CONFIG_H_
#define HLEDMATRIX_CONFIG_H_

/* Direct GPIO connection: rows PC0..PC7, columns PB0..PB7. */
#define HLEDMATRIX_ROW_PORT           PORTC
#define HLEDMATRIX_COLUMN_PORT        PORTB

/* Common-anode matrix: row ON = HIGH, column ON = LOW. */
#define HLEDMATRIX_ROW_ACTIVE_HIGH       1U
#define HLEDMATRIX_COLUMN_ACTIVE_HIGH    0U
#define HLEDMATRIX_ROW_TIME_MS            1UL

#endif /* HLEDMATRIX_CONFIG_H_ */
