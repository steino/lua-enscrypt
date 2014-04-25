local enscryt = assert(require'enscrypt')

local password = "test"
local salt = "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08" -- sha256 of "test"

-- results from enscrypts cli program.
local results = {
	"11e1d357946a72f92cb7be7e4d84a984ca01f808d49c3b20b98c4713a46fb7d7",
	"9a138e8b470c80ea296bb95abc6a25e1d5819c2528af65c8851c1ba26216a729",
	"ed0d4f3c79b8057e00cdbbc959a86f7c4998d99b508b57e4bbb3677d3aac784c",
	"0c9cd73f778edb8f82bbb5c67158bc9ca2d9277a16b0d13441f62551cd924429",
	"d729e19531ad47dd05e458cf4bf80ad7fadd5b3a67831278ffbae59149d5e77c",
	"bb887ecd5c8a54177664634c3518b54ba894eecab16604855c2dca4f8b36c0ba",
	"e31414cb49e9f341b15ccba5e61d4081dede3cd1c11fd8e1ed277bad4e5186e2",
	"10f56e01aa8b7c5d871467f10da925bf9b4a98a0362e8cb60e099c552294a1a3",
	"16a6a6e2b152739e59787c777809d1884a9ec0c3fae80f48057263be39aef836",
	"b97a42b0524e69be4961be6564ba790fcc5fa9d0a973573a0fd0df3fd2049375"
}

local ok, fail = 0, 0

print("Iterations", "Result", "Elapsed")

for i, hash in next, results do
	local output, elapsed = enscrypt.hash(password, salt, i)
	if output == hash then
		ok = ok + 1
		print(i, "", "PASSED", ("%.3fs"):format(elapsed))
	else
		fail = fail + 1
		print(i, "", "FAILED", ("%.3fs"):format(elapsed))
	end
end

local out = "%d test(s) passed, %d test(s) failed"

print(out:format(ok, fail))
