function u_sleep (usec)
  ccall((:u_sleep, "libbase.so"), Void, (Clong,), usec)
end

function safeLogin ()
  ccall((:safeLogin, "libbase.so"), Void, ())
end

function safeSetTrack ()
  ccall((:safeSetTrack, "libbase.so"), Void, ())
end

function safeGetNumberOfProblems ()
  return ccall((:safeGetNumberOfProblems, "libbase.so"), Cint, ())
end

function safeSetProblem (problemID::Cint)
  ccall((:safeSetProblem, "libbase.so"), Void, (Cint,), problemID)
end

function safeEvaluate (problemID::Cint, point::Array{Cdouble, 1},
    value::Array{Cdouble, 1})
  ccall((:safeEvaluate, "libbase.so"), Void, (Cint, Ptr{Cdouble}, Ptr{Cdouble}),
      problemID, point, value)
end

function safeGetDimension (problemID::Cint)
  return ccall((:safeGetDimension, "libbase.so"), Cint, (Cint,), problemID)
end

function safeGetBudget (problemID::Cint)
  return ccall((:safeGetBudget, "libbase.so"), Cint, (Cint,), problemID)
end

function safeGetEvaluations (problemID::Cint)
  return ccall((:safeGetEvaluations, "libbase.so"), Cint, (Cint,), problemID)
end
