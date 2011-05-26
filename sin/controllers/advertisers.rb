get '/advertisers/create' do
  protected!
  advertiser = Advertiser.new
  advertiser.user_id = session[:id]
  advertiser.save

  redirect '/advertisers/index'
end

get '/advertisers/index' do
  protected!

  @advertiser = Advertiser.first(:user_id => session[:id])

  if @advertiser.nil? then
    redirect '/advertisers/create'
  else

    @campaigns = Campaign.all(:user_id => session[:id])
    @totalcamps = Campaign.count(:user_id => session[:id])

    haml :"advertisers/index"
  end
end
