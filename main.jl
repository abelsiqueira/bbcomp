trial = true

if trial
  include("src/trial.jl")
else
  include("src/code.jl")
end
include("src/method.jl")

if !trial
  ccall( (:dll_load, "libbase.so"), Void, ())
end

safeLogin()
safeSetTrack()
n = safeGetNumberOfProblems()

for i = 0:n-1
  solveProblem(i)
end

if !trial
  ccall( (:dll_close, "libbase.so"), Void, ())
end
