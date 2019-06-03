/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel  
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  Copyright (C) 2005 by Freelines CO.,Ltd
 * 
 *  Copyright (C) 2010 by Meika Sugimoto
 *
 *  �嵭����Ԥϡ��ʲ��� (1)���(4) �ξ�狼��Free Software Foundation 
 *  �ˤ�äƸ�ɽ����Ƥ��� GNU General Public License �� Version 2 �˵�
 *  �Ҥ���Ƥ���������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ�����
 *  ����Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����
 *  �ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ
 *  ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 */

#define	TOPPERS_MACRO_ONLY
#include "kernel_impl.h"
#include "v850asm.inc"

	.section	".text" , "ax"
	.align	4

/*
 *	V850�ѥ������ȥ��åץե�����
 */
	.global	hardware_init_hook
	
	.globl	_start
	.type	_start, @function
_start:
	di

	/*
	 * �Ƽ�ݥ��󥿤ν����(SP/TP/EP/GP/CTBP)
	 */
	Lea		__kernel_istkpt, r3
	ld.w	0[r3], r3

	Lea		__text , tp
	Lea		__ep , ep
	Lea 	__gp , gp
	Lea		__ctbp , r6
	ldsr	r6 , 20
	
	/*
	 *  hardware_init_hook �θƽФ���0 �Ǥʤ�����
	 *
	 *  �������åȥϡ��ɥ������˰�¸����ɬ�פʽ����������������
	 *  �ϡ�hardware_init_hook �Ȥ����ؿ����Ѱդ��롥
	 */

	Lea		hardware_init_hook, r1
	cmp		r0, r1
	be		hardware_init_hook_end
	jarl	hardware_init_hook, lp
hardware_init_hook_end:
	
	/*
	 *  bss���������Υ��ꥢ
	 *   ���饤�󤵤�Ƥ���Τ�4���ĹԤ�
	 */
	Lea		__bss_start, ep
	Lea		__bss_end, r6
bss_clear_start:
	cmp		ep, r6
	be		bss_clear_end
	sst.w	r0, 0[ep]
	add		4, ep
	br		bss_clear_start
bss_clear_end:

	/*
	 *  data���������ν������ROM���б���
	 *
	 *  __idata_start ���� __idata_end �ޤǤ�__data_start �ʹߤ�
	 *  ���ԡ����롥
	 */
	Lea		__idata_start, r6
	Lea		__idata_end,   r7
	Lea		__data_start,  r8
data_init_start:
	cmp		r6, r7
	be		data_init_end
	ld.w	0[r6], r9
	st.w	r9, 0[r8]
	add		4, r6
	add		4, r8
	br		data_init_start
data_init_end:

	/*
	 *  software_init_hook ��ƽФ���0 �Ǥʤ�����
	 *
	 *  ���եȥ������Ķ����ä˥饤�֥��ˤ˰�¸����ɬ�פʽ������
	 *  ����������ϡ�software_init_hook �Ȥ����ؿ����Ѱդ����
	 *  �褤��
	 */

	Lea		software_init_hook, r1
	cmp		r0, r1
	be		software_init_hook_end
	jarl	software_init_hook, r31
software_init_hook_end:

	/*
	 *  �����ͥ��ư���롥
	 */
	.globl	_sta_ker
	jarl	_sta_ker, r31
	halt

	.end