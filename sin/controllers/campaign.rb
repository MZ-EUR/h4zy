post '/campaigns/create' do
  protected!

  campaign = Campaign.new
  campaign.adtype_id = params[:adtype]
  campaign.user_id = session[:id]
  campaign.title = params[:title]
  campaign.save

  redirect '/advertisers/index'
end

get '/campaigns/show/:id' do
  protected!
  @campaign = Campaign.get(params[:id])

  redirect request.referer if @campaign.nil?

  if @campaign.adtype.cpv then
    haml :"campaigns/cpv-list"
  else
 
    @ads = @campaign.ads
 
    haml :"campaigns/show"
  end

end
