#
#	This file provides the TCL procedure commentOutSDCFile which takes one
#	input argument that is the name of an SDC file that we want to comment out.
#	This script verifies that the requested file exists, then it checks to see
#	if the file has already been commented out, and if not, it creates a new
#	file with TCL comments at the top and a post_message to inform users that
#	the file has been intentionally commented out.  Then the body of the
#	original file is copied in followed by the closing brace for the commented
#	out section.  The new file is then copied over the original file.
#
post_message -type info "Reading file: \'commentOutSDCFile.sdc\'"

proc commentOutSDCFile SDCFileName {

	set FILE_TO_COMMENT_NAME ${SDCFileName}

		if { [ file exists ${FILE_TO_COMMENT_NAME} ] } {
		
		set FILE_TO_COMMENT_CHANNEL [ open ${FILE_TO_COMMENT_NAME} "r" ]
		
		if { [ gets ${FILE_TO_COMMENT_CHANNEL} nextLine ] >= 0 } {
			
			set firstLine "post_message -type info \"This entire file has been intentionally commented out...\""
			
			if { ! [ string equal ${nextLine} ${firstLine} ] } {
				
				set tempFileChannel [ open ${FILE_TO_COMMENT_NAME}\.temp "w" ]
				post_message -type info "Commenting out file \"${FILE_TO_COMMENT_NAME}\"..."
							
				puts ${tempFileChannel} ${firstLine}
				puts ${tempFileChannel} " "
				puts ${tempFileChannel} "if { 0 } {"
				puts ${tempFileChannel} " "
				puts ${tempFileChannel} "error_do_not_use_this_file {} "
				puts ${tempFileChannel} " "
				puts ${tempFileChannel} "#"
				puts ${tempFileChannel} "# This entire file has been intentionally commented out..."
				puts ${tempFileChannel} "#"
				puts ${tempFileChannel} " "
				puts ${tempFileChannel} ${nextLine}
				while { [ gets ${FILE_TO_COMMENT_CHANNEL} nextLine ] >= 0 } {
					if { [ string length ${nextLine} ] == 0 } {
						puts ${tempFileChannel} " "
					} else {
						puts ${tempFileChannel} ${nextLine}
					}
				}

				puts ${tempFileChannel} " "
				puts ${tempFileChannel} "}"
				puts ${tempFileChannel} " "
				
				close ${tempFileChannel}
				close ${FILE_TO_COMMENT_CHANNEL}

				file rename -force ${FILE_TO_COMMENT_NAME}.temp ${FILE_TO_COMMENT_NAME} 
			} else {
				close ${FILE_TO_COMMENT_CHANNEL}
			}
		} else {
			close ${FILE_TO_COMMENT_CHANNEL}
			post_message -type info "File to comment \"${FILE_TO_COMMENT_NAME}\" appears to be empty..."
		}
	} else {
		post_message -type info "File to comment \"${FILE_TO_COMMENT_NAME}\" does NOT exist..."
	}
}
