def Settings( **kwargs ):
  return {
    'flags': [ '-x', 
		'c',
		 '-Wall',
		 '-Wextra',
		 '-Werror',
#		 '-I','CMSIS/inc',
#		 '-I','Code/inc',
#	 	 '-I','FreeRTOS/include',
		    ],
  }
