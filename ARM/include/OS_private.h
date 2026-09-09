#ifndef OS_PRIVATE_H_
#define OS_PRIVATE_H_

/* One periodic task managed by the simple cooperative scheduler. */
typedef struct
{
    void (*TASK_PTR)(void);
    u16 TASK_Periodicity;
} TASK;

#endif /* OS_PRIVATE_H_ */
