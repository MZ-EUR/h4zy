get '/ads/destroy/:id' do
  protected!
  
  ad = Ad.get(params[:id])
  cid = ad.campaign_id
  ad.destroy!

  redirect "/campaigns/show/#{cid}"
end

get '/ads/destroyurl/:id' do
  protected!
  
  url = Url.get(params[:id])
  cid = url.campaign_id
  url.destroy!

  redirect "/campaigns/show/#{cid}"
end

get '/ads/destroykeyword/:id' do
  protected!
  
  kw = Keyword.get(params[:id])
  cid = kw.campaign_id
  kw.destroy!

  redirect "/campaigns/show/#{cid}"
end

post '/ads/newkeywords' do
  protected!

  if !params[:kwlist].nil? && !params[:kwlist].empty? then
    keywords = params[:kwlist].split("\r\n")

    keywords.each do |rkw|

      kw = Keyword.new
      kw.campaign_id = params[:cid]
      kw.keyword = rkw
      kw.user_id = session[:id]
      kw.save

    end
  end

  if !params[:filename].nil? && !params[:filename].empty? then

    contents = params[:filename][:tempfile].read
    keywords = contents.split("\n")

    keywords.each do |rkw|

      kw = Keyword.new
      kw.campaign_id = params[:cid]
      kw.url = rkw
      kw.user_id = session[:id]
      kw.save

    end
 
  end

  redirect "/campaigns/show/#{params[:cid]}"
end

post '/ads/newurls' do
  protected!

  if !params[:urllist].nil? && !params[:urllist].empty? then
    urls = params[:urllist].split("\r\n")

    urls.each do |rurl|

      if isdomain(rurl) then
        url = Url.new
        url.campaign_id = params[:cid]
        url.url = rurl
        url.user_id = session[:id]
        url.save
      else
        session[:error] = "Some of your urls were not urls."
      end
    end
  end

  if !params[:filename].nil? && !params[:filename].empty? then

    contents = params[:filename][:tempfile].read
    urls = contents.split("\n")

    urls.each do |rurl|

      if isdomain(rurl) then
        url = Url.new
        url.campaign_id = params[:cid]
        url.url = rurl
        url.user_id = session[:id]
        url.save
      else
        session[:error] = "Some of your urls were not urls."
      end
    end
 
  end

  redirect "/campaigns/show/#{params[:cid]}"
end

# should get cookie sent for affiliate-id, identity
get '/ads/bysocial' do
  @socialads = Ad.social

  identity = request.cookies["identity"]
  affid = request.cookies["AffiliateID"]
  affid ||= Affiliate.first.id
 
  host = Host.first(:identity => identity)

  ua, remote_addr = req_info(request)

  host = host_create(affid, remote_addr, ua) unless host

  @socialads.each do |ad|
    impression = Impression.new
    impression.ad_id = ad.id
    impression.host_id = host.id

    if affid.empty? then
      impression.affiliate_id = 0
    else
      impression.affiliate_id = affid
    end

    impression.save
  end

  # reset identity cookie -- just in case we had to make a new host
  dt = (Time.now + 24*60*60*365*5)
  response.set_cookie("identity", {:value => "#{host.identity}", :expires => dt})
 
  erb :"ads/bysocial"
end

get '/ads/byurl' do

  rstr = ''

  if !params[:GetAd].nil? then
    affid = params[:AffiliateID]
    affid ||= Affiliate.first.id
 
    identity = params["identity"]
    identity ||= ''

    host = Host.first(:identity => identity)

    ua, remote_addr = req_info(request)

    host = host_create(affid, remote_addr, ua) unless host

    # set identity cookie if unknown
    if host.unknown then
      dt = (Time.now + 24*60*60*365*5)
      response.set_cookie("identity", {:value => "#{host.identity}", :expires => dt})
    end

    ad = Ad.first

    impression = Impression.new
    impression.ad_id = ad.id
    impression.affiliate_id = affid.to_i # will default to 0 if affiliateid was nil
    impression.host_id = host.id
    impression.save

    rstr = "Callback({\"Response\":[{\"PopupDomain\":[{\"Description\":\"#{ad.content}\",\"Link\":\"#{ad.trackinglink}\"}]}]})"
  end

  if !params[:GetAds].nil? then
    rstr = "Callback({\"Response\":[{\"PopupDomain\":[{\"Description\":\"ad 1 txt\",\"Link\":\"http://www.ad1.com\"},{\"Description\":\"ad 2 txt\",\"Link\":\"http://www.ad2.com\"},{\"Description\":\"ad 3 txt\",\"Link\":\"http://www.ad3.com\"}]}]})"
  end

  rstr
end

# should get cookie sent for affiliate-id, identity
get '/ads/cpv' do

  identity = request.cookies["identity"]
  affid = request.cookies["AffiliateID"]
  affid ||= Affiliate.first.id
 
  host = Host.first(:identity => identity)

  ua, remote_addr = req_info(request)

  host = host_create(affid, remote_addr, ua) unless host

  # log impressions

  # reset identity cookie -- just in case we had to make a new host
  dt = (Time.now + 24*60*60*365*5)
  response.set_cookie("identity", {:value => "#{host.identity}", :expires => dt})

  'http://startrecoveringnow.com/3.php?p=1061&i=430001&s='
end

get '/ads/bykw' do
end

get '/ads/track' do

  ad = Ad.first(:ad_link => params[:ad])

  identity = request.cookies["identity"]
  affid = request.cookies["AffiliateID"]
  affid ||= params[:AffiliateID]
  affid ||= Affiliate.first.id
 
  # does NOT work for IE yet, cause the IE plugin does not persist/pass
  # along the host-id
  host = Host.first(:identity => identity)

  ua, remote_addr = req_info(request)

  host = host_create(affid, remote_addr, ua) unless host

  # set identity cookie if unknown
  if host.unknown then
    dt = (Time.now + 24*60*60*365*5)
    response.set_cookie("identity", {:value => "#{host.identity}", :expires => dt})
  end

  click = Click.new
  click.ad_id = ad.id
  click.affiliate_id = host.affiliate_id
  click.host_id = host.id
  click.save

  redirect ad.link
end

get '/ads/new/:id' do
  protected!
  @campaign = Campaign.get(params[:id])

  haml :"ads/new"
end

post '/ads/create' do
  protected!

  if isdomain(params[:link]) then

    ad = Ad.new
    ad.user_id = session[:id]
    ad.campaign_id = params[:campaign]
    ad.title = params[:title]
    ad.link = params[:link]
    ad.content = params[:content]

    if !params[:image].nil? && !params[:image].empty? then
      # heat shrink these images down to 110x85
      fname = session[:id].to_s + "-" + Time.now.to_i.to_s + ".jpg"
      fpath = params[:image][:tempfile]

      resizeimg(fpath.path, fname)
      stores3(fname, '/tmp/' + fname)
  
      ad.pic = fname
    end

    
    if ad.save
      session[:message] = "Created new ad."
      redirect "/campaigns/show/#{params[:campaign]}"
    else
      session[:error] = "#{ad.errors.first.to_s}"
      redirect "/ads/new/#{params[:campaign]}"
    end

  else
    session[:error] = "Some of your urls were not urls."
    redirect "/ads/new/#{params[:campaign]}"
  end

end
