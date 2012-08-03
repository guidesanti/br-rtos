	RSEG    CODE:CODE(2)
	thumb

	EXTERN runningTask
	EXTERN __BR_TaskSwitch

	PUBLIC __BR_PortPendSVCHandler
	PUBLIC __BR_PortEnableInterrupts
	PUBLIC __BR_PortDisableInterrupts
	PUBLIC __BR_PortSVCHandler
	PUBLIC __BR_PortSetBasePriorityMask
	PUBLIC __BR_PortStartFirstTask

__BR_PortPendSVCHandler:
	/* Get the running task stack pointer. */
	mrs r0, psp
	/* Get the current running task.
	 * The first element of the task is the task stack pointer.
	 */
	ldr	r3, =runningTask
	ldr	r2, [r3]
	/* Save the remaining resgisters: R4 to R11. */
	stmdb r0!, { r4-r11 }
	/* Save the new stack pointer into task->stackPointer. */
	str r0, [r2]

	/* Save R3 to R14 temporarily on the main stack. */
	stmdb sp!, { r3, r14 }
	mov r0, #15
	msr basepri, r0
	bl __BR_TaskSwitch
	mov r0, #0
	msr basepri, r0
	/* Restore R3 to R14. */
	ldmia sp!, { r3, r14 }

	/* Put the new task to run. */
	ldr r1, [r3]
	ldr r0, [r1]
	ldmia r0!, { r4-r11 }
	msr psp, r0

	bx r14

/*
 * void __BR_PortEnableInterrupts(void)
 */
__BR_PortEnableInterrupts:
	cpsie i
	bx r14

/*
 * void __BR_PortDisableInterrupts(void)
 */
__BR_PortDisableInterrupts:
	cpsid i
	bx r14

/*
 * void BR_PortSetBasePriorityMask(uint8_t basePriMask)
 * R0 -> basePriMask
 */
__BR_PortSetBasePriorityMask:
	msr primask, r0

	bx lr

/*
 * void BR_PortSVCHandler(void)
 *
 * The SVC handler runs on exception/provileged mode, so here we can execute
 * a context switch.
 * Get the first tast to run from runningTask pointer and put it to run
 * using the process stack (psp).
 */
__BR_PortSVCHandler:
	ldr	r3, =runningTask
	ldr r1, [r3]
	ldr r0, [r1]
	ldmia r0!, {r4-r11}
	msr psp, r0
	mov r0, #0
	msr	basepri, r0
	orr r14, r14, #13

	bx r14

/*
 * void BR_PortStartFirstTask(void)
 */
__BR_PortStartFirstTask:
	/* Set the msp back to the start of the stack. */
	ldr r0, =0xE000ED08
	ldr r0, [r0]
	ldr r0, [r0]
	msr msp, r0
	/* Enable the interrupts and call the SVC handler. */
	cpsie i
	svc 0

	END
