/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 log.h												:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: abombard <marvin@42.fr>					+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2016/04/01 15:24:50 by abombard		   #+#	  #+#			  */
/*	 Updated: 2016/04/01 15:44:10 by abombard		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# include <stdio.h>
# include "types.h"

# define LF ":%s:%s():%d#"
# define LA __FILE__, __func__, __LINE__

# define LOG_ERROR(FMT,...) fprintf(stderr,"FATAL"LF FMT"\n",LA,__VA_ARGS__)
# define LOG_WARNING(FMT,...) fprintf(stderr,"WARNING"LF FMT"\n",LA,__VA_ARGS__)
# define LOG_DEBUG(FMT,...) fprintf(stderr,"DEBUG"LF FMT"\n",LA,__VA_ARGS__)

# define FATAL(FMT, ...) do{LOG_ERROR(FMT,__VA_ARGS__);return FALSE;}while (0)

# define ASSERT(expr) if (!(expr)) FATAL("(%s) failed\r", #expr)

#endif
