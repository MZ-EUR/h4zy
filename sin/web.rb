require 'rubygems'
require 'sinatra'
require 'haml'
require 'dm-core'
require 'dm-validations'

require 'base64'  # seriously? this is not included in any of these fucking gems!?
require 'user-agent'

require 'geokit'
require 'aws/s3'
require 'RMagick'

require 'db-config.rb'

# load in all of our models
Dir.glob('models/*').each do |f|
  require f
end

enable :sessions

CONTENT_TYPES = {:html => 'text/html', :css => 'text/css', :js  => 'application/javascript'}

before do
  request_uri = case request.env['REQUEST_URI']
    when /\.css$/ : :css
    when /\.js$/  : :js
    else          :html
  end
  content_type CONTENT_TYPES[request_uri], :charset => 'utf-8'

  if !session[:error].nil? then
    @msgerror = session[:error]
    session[:error] = nil
  end

  if !session[:notice].nil? then
    @msgnotice = session[:notice]
    session[:notice] = nil
  end

end

set :haml, {:format => :html5 }

not_found do
  haml :'404', :layout => :front_layout
end

# grab all of our controllers
Dir.glob('controllers/*').each do |f|
  require f
end

# grab all of our helpers
Dir.glob('helpers/*').each do |f|
  require f
end
