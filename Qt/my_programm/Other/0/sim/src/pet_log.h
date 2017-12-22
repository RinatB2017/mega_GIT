/**
 * 17/08/2016
 */

#ifndef _PET_LOG_H
#define _PET_LOG_H


/**
 * Маркеры важности
 */
#define PETLOG_EMERG    0  /* A panic condition */
#define PETLOG_ALERT    1  /* A condition that should be corrected immediately, such as a corrupted system database */
#define PETLOG_CRIT     2  /* Critical conditions, such as hard device errors */
#define PETLOG_ERROR    3  /* Errors*/
#define PETLOG_WARNING  4  /* Warning messages */
#define PETLOG_NOTICE   5  /* Conditions that are not error conditions, but that may require special handling */
#define PETLOG_INFO     6  /* Informational messages */
#define PETLOG_DEBUG    7  /* Messages that contain information normally of use only when debugging a program */


/**
 * Open a connection to the logging facility
 */
void pet_log_open(const char *ident, int logopt, int facility);


/**
 * Close a connection to the logging facility
 */
void pet_log_close(void);

/**
 * Set log priority mask
 */
int pet_log_setmask(int maskpri);


/**
 * Log a message
 */
void pet_log(int priority, const char *message, ... /* arguments */);


#endif /* _PET_LOG_H */
