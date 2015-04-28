include("src/code.jl")

ccall( (:dll_load, "libbase.so"), Void, ())

safeLogin()
safeSetTrack()
n = safeGetNumberOfProblems()

solveProblem(0)

ccall( (:dll_close, "libbase.so"), Void, ())
