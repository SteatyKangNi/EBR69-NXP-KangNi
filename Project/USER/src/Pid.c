


		errorr_last = errorr;
        errorl_last =  errorl;
		errorl = spd_set - templ_pluse;
		errorr = spd_set - tempr_pluse;
		errorl_i = errorl_i + errorl;
		errorr_i = errorr_i + errorr;
		errorr_d = errorr_last - errorr;
		errorl_d = errorl_last - errorl;
		dutyl = dutyl + 0.02*errorl + 0.001*errorl_i + 0.01*errorl_d;
		dutyr = dutyr + 0.02*errorr + 0.001*errorr_i + 0.01*errorr_d;