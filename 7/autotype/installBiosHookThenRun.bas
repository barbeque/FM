10 OPEN "I",#1,"COM0:(F8N1)"
20 LINE INPUT #1,A$
40 CLOSE
50 EXEC VARPTR(A$)
60 RUN ""
RUN
