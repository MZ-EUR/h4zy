@user = "devuser"
@pass = "blah blah blah"
@host = "localhost"

if !ENV['production'].nil? then
  @pass = "free password daaaaaayy"
end

DataMapper.setup(:default, "mysql://#{@user}:#{@pass}@#{@host}/hazy")
DataMapper.finalize
