post '/payments/create' do
  admin!

  user = User.first(:login => params[:user])
  
  if !user.nil?

    amount = dollars_to_cents(params[:amount])
    
    user.balance += amount
    user.save

    payment = Payment.create
    payment.user_id = user.id
    payment.amount = amount
    payment.advertiser_id = user.advertiser.id
    payment.save

    session[:notice] = "Added #{params[:amount]} to user's balance."

  else
    session[:error] = "Could not find user!"

  end

  redirect "/admin/payments"

end
