require 'migrate.rb'
require 'spec'

Dir.glob('test/*').each do |f|
  require f
end
