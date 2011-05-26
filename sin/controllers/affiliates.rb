get '/affiliates/create' do
  protected!
  affiliate = Affiliate.new
  affiliate.user_id = session[:id]
  affiliate.save

  redirect '/affiliates/index'
end

get '/affiliates/index' do
  protected!

  @affiliate = Affiliate.first(:user_id => session[:id])

  if @affiliate.nil? then
    redirect '/affiliates/create'
  else

    @totalhosts = Host.count(:affiliate_id => session[:id])
    @hosts = Host.all(:affiliate_id => session[:id])

    haml :"affiliates/index"
  end
end
