// osplink.h - sending telegrams from the PC (UART over USB to ESP)
/*****************************************************************************
 * Copyright 2024 by ams OSRAM AG                                            *
 * All rights are reserved.                                                  *
 *                                                                           *
 * IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
 * THE SOFTWARE.                                                             *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
 *****************************************************************************/
#ifndef _OSPLINK_H_
#define _OSPLINK_H_


// Application version (and its history)
#define OSPLINK_VERSION "1.7"
// 20240628  1.7  Added topo command (aomw)
// 20240522  1.6  Fixed bug in aocmd_file_bootcmd_exec_on_por() 
// 20240517  1.5  Added python lib
// 20240517  1.4  Added said command (taking otp, adding i2c)
// 20240516  1.3  Added osp otp command
// 20240515  1.2  Added osp tx
// 20240515  1.1  Added osp send
// 20240514  1.0  Created


// Application long name
#define OSPLINK_LONGNAME "OSPlink"


// Application banner
#define OSPLINK_BANNER "\n\n\n\n"\
  "  ____   _____ _____    _ _       _\n"\
  " / __ \\ / ____|  __ \\  | (_)     | |\n"\
  "| |  | | (___ | |__) | | |_ _ __ | | __\n"\
  "| |  | |\\___ \\|  ___/  | | | '_ \\| |/ /\n"\
  "| |__| |____) | |      | | | | | |   <\n"\
  " \\____/|_____/|_|      |_|_|_| |_|_|\\_\\\n"\
  // https://patorjk.com/software/taag/#p=display&v=2&f=Big&t=OSP%20link


#endif


                                        
                                    