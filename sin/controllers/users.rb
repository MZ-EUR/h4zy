get '/walkthrough' do
  haml :"users/walkthrough", :layout => :front_layout
end

get '/newaccount' do
  haml :"users/new", :layout => :front_layout
end

post '/users/create' do
  u = User.new
  u.login = params[:login]
  u.email = params[:email]
  u.password = params[:password]
  u.save

  session[:id] = u.id

  redirect '/dashboard'
end

post '/auth' do
  u = User.authenticate(params[:login], params[:password])

  if !u.nil? then
    session[:id] = u.id
    redirect '/dashboard'
  else
    redirect '/'
  end
end

get '/dashboard' do
  protected!
  haml :"users/dashboard"
end

get '/logout' do
  session[:id] = nil
  redirect '/'
end
