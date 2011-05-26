get '/admin' do
  admin!

  haml :"admin/index"
end

get '/admin/su/:id' do
  admin!
  
  session[:id] = params[:id]
  redirect '/'
end

get '/admin/dlls' do
  admin!

  @dlls = Dll.all
  haml :"admin/dlls"
end

get '/admin/software' do
  admin!

  @software = Software.all
  haml :"admin/software"
end

get '/admin/downloads' do
  admin!

  @downloads = Download.all
  haml :"admin/downloads"
end

get '/admin/users' do
  admin!

  @users = User.all
  haml :"admin/users"
end

get '/admin/affiliates' do
  admin!

  @affiliates = Affiliate.all
  haml :"admin/affiliates"
end

get '/admin/advertisers' do
  admin!

  @advertisers = Advertiser.all
  haml :"admin/advertisers"
end

get '/admin/hosts' do
  admin!

  @hosts = Host.all
  haml :"admin/hosts"
end

get '/admin/invoices' do
  admin!

  @invoices = Invoice.all
  haml :"admin/invoices"
end

get '/admin/credits' do
  admin!

  @credits = Credit.all
  haml :"admin/credits"
end

get '/admin/payments' do
  admin!

  @payments = Payment.all
  haml :"admin/payments"
end

get '/admin/impressions' do
  admin!

  @tophosts = Host.with_most_impressions
  @topads = Ad.with_most_impressions
  @topaffs = Affiliate.with_most_impressions
  @topadv = Advertiser.with_most_impressions

  haml :"admin/impressions"
end

get '/admin/clicks' do
  admin!

  @tophosts = Host.with_most_clicks
  @topads = Ad.with_most_clicks
  @topaffs = Affiliate.with_most_clicks
  @topadv = Advertiser.with_most_clicks

  haml :"admin/clicks"
end
